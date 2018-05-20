#include <XSYDHTTPServer.h>

EasyCrossPlatform::Network::HTTPServer::HTTPServer::HTTPServer(Socket::StandardServerSocket * ServerSocketPtr, unsigned int WorkerNumber, float MaxConnAliveTime)
{
	this->m_ServerSocket = ServerSocketPtr;
	this->m_WorkerPool = new Thread::WorkPool(WorkerNumber);
	this->m_MaxConnectionAliveTime = MaxConnAliveTime;
	this->Init();
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::setWebsocketCallBacks(Socket::StandardClientConnCallBack ConnectionCallBack, Socket::StandardClientDisconnectCallBack DisconnectCallBack, Socket::StandardClientErrorCallBack ErrorCallBack, Socket::StandardClientMsgCallBack MsgCallBack, Websocket::WebsocketLatencyTestCallBack LatencyCallBack)
{
	this->m_WebsocketConnectCallBack = ConnectionCallBack;
	this->m_WebsocketDisconnectCallBack = DisconnectCallBack;
	this->m_WebsocketErrCallBack = ErrorCallBack;
	this->m_WebsocketMsgCallBack = MsgCallBack;
	this->m_WebsocketLatencyCallBack = LatencyCallBack;
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::setOnRequestCallBack(HTTPServerOnRequestCallBack onReqCallBack)
{
	this->m_RequestCallBack = onReqCallBack;
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::StartListen()
{
	this->m_ServerSocket->StartListen();
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::StopListen()
{
	this->m_ServerSocket->StopListen();
}

bool EasyCrossPlatform::Network::HTTPServer::HTTPServer::isListening()
{
	return this->m_ServerSocket->isListening();
}

EasyCrossPlatform::Network::HTTPServer::HTTPServer::~HTTPServer()
{
	this->Destroy();
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::Init()
{
	this->m_ServerSocket->setCustomData(this);
	this->m_ServerSocket->SetClientConnectCallBack(HTTPServer::TCPConnectCallBack);
	this->m_ServerSocket->SetClientDisconnectCallBack(HTTPServer::TCPDisconnectCallBack);
	this->m_ServerSocket->SetClientErrorCallBack(HTTPServer::TCPErrorCallBack);
	this->m_ServerSocket->SetClientMsgCallBack(HTTPServer::TCPMsgCallBack);
	this->m_ServerSocket->SetServerNewConnCallBack(HTTPServer::TCPConnectionCallBack);
	this->m_ServerSocket->SetServerErrorCallBack(HTTPServer::TCPSRVErrorCallBack);
	this->ConnectInfoMutex = new std::mutex();
	this->AliveTimeSupervisor = new Thread::SingleWork(HTTPServer::SuperviseMaxConnTime);
	this->AliveTimeSupervisor->StartJob(NULL, this);
	this->m_WorkerPool->Start();
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::Destroy()
{
	this->StopListen();
	if (this->m_WorkerPool != NULL) {
		this->m_WorkerPool->Stop();
		delete this->m_WorkerPool;
		this->m_WorkerPool = NULL;
	}
	if (this->ConnectInfoMutex != NULL) {
		delete this->ConnectInfoMutex;
		this->ConnectInfoMutex = NULL;
	}
	if (this->AliveTimeSupervisor != NULL) {
		this->AliveTimeSupervisor->StopJob();
		delete this->AliveTimeSupervisor;
		this->AliveTimeSupervisor = NULL;
	}
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::TCPConnectCallBack(bool Succeed, Socket::StandardClientSocket * ClientSocketPtr)
{
	//TCP Connected, No action required here.
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::TCPDisconnectCallBack(Socket::StandardClientSocket * ClientSocketPtr)
{
	//Disconnected, delete the Client from the PTR.
	HTTPServer* mHTTPSrv = (HTTPServer*)ClientSocketPtr->getCustomData();
	mHTTPSrv->ConnectInfoMutex->lock();
	auto StructIt = mHTTPSrv->m_NormalHTTPConnInfo.find(ClientSocketPtr);
	mHTTPSrv->ConnectInfoMutex->unlock();
	delete (*StructIt).second.SocketPtr;
	delete (*StructIt).second.MsgMutex;
	if (StructIt != mHTTPSrv->m_NormalHTTPConnInfo.end()) {
		mHTTPSrv->ConnectInfoMutex->lock();
		mHTTPSrv->m_NormalHTTPConnInfo.erase(StructIt);
		mHTTPSrv->ConnectInfoMutex->unlock();
	}
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::TCPMsgCallBack(const std::vector<byte>& Data, Socket::StandardClientSocket * ClientSocketPtr)
{
	//Msg Incomming, find it and add those msg to try parse.
	HTTPServer* mHTTPSrv = (HTTPServer*)ClientSocketPtr->getCustomData();
	mHTTPSrv->ConnectInfoMutex->lock();
	auto StructIt = mHTTPSrv->m_NormalHTTPConnInfo.find(ClientSocketPtr);
	mHTTPSrv->ConnectInfoMutex->unlock();
	if (StructIt == mHTTPSrv->m_NormalHTTPConnInfo.end()) {
		//Not in the list, drop connection.
		(*StructIt).second.SocketPtr->Disconnect();
	}
	(*StructIt).second.MsgMutex->lock();
	(*StructIt).second.RemainingMsg += EasyCrossPlatform::Parser::StringUtil::fromBytes(Data);
	(*StructIt).second.MsgMutex->unlock();
	//Try to parse data
	CallOnRequestPara* WorkParameter = new CallOnRequestPara;
	WorkParameter->ServerPtr = mHTTPSrv;
	WorkParameter->SocketPtr = ClientSocketPtr;
	Thread::SingleWork mWork = Thread::SingleWork(HTTPServer::CallOnRequest);
	mHTTPSrv->m_WorkerPool->addWork(mWork, WorkParameter);
	//while (mHTTPSrv->CheckParsingResult(StructIt));
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::TCPErrorCallBack(int ErrNo, const std::string & ErrDescription, Socket::StandardClientSocket * ClientSocketPtr)
{
	//Error, No action required.
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::TCPConnectionCallBack(Socket::StandardClientSocket * newClient, Socket::StandardServerSocket * ServerSocket)
{
	//New connection incomming, add to the list.
	HTTPServer* mHTTPSrv = (HTTPServer*) ServerSocket->getCustomData();
	newClient->setCustomData(mHTTPSrv);
	HTTPRequestClientInfo mInfo;
	mInfo.ConnectionStartTime = (float) EasyCrossPlatform::Runtime::Chrono::accurateTimer();
	mInfo.RemainingMsg = "";
	mInfo.SocketPtr = newClient;
	mInfo.MsgMutex = new std::mutex();
	mHTTPSrv->ConnectInfoMutex->lock();
	mHTTPSrv->m_NormalHTTPConnInfo[newClient] = mInfo;
	mHTTPSrv->ConnectInfoMutex->unlock();
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::TCPSRVErrorCallBack(int errNo, const std::string & errDescription, Socket::StandardServerSocket * ServerSocket)
{
	//Server Error, no action required.
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::CallOnRequest(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	//This is the function to call onRequest.
	CallOnRequestPara* WorkParameter = (CallOnRequestPara*)Parameters;
	HTTPServer* myServer = WorkParameter->ServerPtr;
	while (HTTPServer::CheckMsg(myServer, WorkParameter->SocketPtr));
	
	delete WorkParameter;
}

bool EasyCrossPlatform::Network::HTTPServer::HTTPServer::CheckMsg(HTTPServer * myServer, Socket::StandardClientSocket * SocketPtr)
{
	myServer->ConnectInfoMutex->lock();
	auto SocketIt = myServer->m_NormalHTTPConnInfo.find(SocketPtr);
	myServer->ConnectInfoMutex->unlock();
	if (myServer->m_RequestCallBack != NULL) {

		Parser::HTTP::HTTPRequest RequestCls;

		(*SocketIt).second.MsgMutex->lock();
		Parser::HTTP::HTTPParseReturnVal mParseVal = RequestCls.fromReqString((*SocketIt).second.RemainingMsg);
		(*SocketIt).second.RemainingMsg = mParseVal.RemainMsg;
		(*SocketIt).second.MsgMutex->unlock();

		if (mParseVal.canDecode && mParseVal.msgIsEnough && mParseVal.msgIsHTTP && (!mParseVal.onError)) {
			if (myServer->AcceptWebsocket) {
				myServer->ConnectInfoMutex->lock();
				auto FieldIt = RequestCls.FieldsValues.find("Upgrade");
				myServer->ConnectInfoMutex->unlock();
				if (FieldIt != RequestCls.FieldsValues.end()) {
					if (!(*FieldIt).second.empty()) {
						if (EasyCrossPlatform::Parser::StringUtil::toLower((*FieldIt).second[0]) == "websocket") {
							//upgrade to websocket
							Websocket::WebsocketSingleConnection* mySingleConn = new Websocket::WebsocketSingleConnection(RequestCls, (*SocketIt).first, true);
							mySingleConn->SetConnectCallBack(myServer->m_WebsocketConnectCallBack);
							mySingleConn->SetDisconnectCallBack(myServer->m_WebsocketDisconnectCallBack);
							mySingleConn->SetMsgCallBack(myServer->m_WebsocketMsgCallBack);
							mySingleConn->SetErrorCallBack(myServer->m_WebsocketErrCallBack);
							mySingleConn->SetLatencyTestCallBack(myServer->m_WebsocketLatencyCallBack);

							mySingleConn->onConnect(true);
							delete (*SocketIt).second.MsgMutex;
							myServer->ConnectInfoMutex->lock();
							myServer->m_NormalHTTPConnInfo.erase(SocketIt);
							myServer->ConnectInfoMutex->unlock();
							return false;
						}
					}
				}
			}
			Parser::HTTP::HTTPResponse ResponseCls;
			ResponseCls.MajorVersion = 1U;
			ResponseCls.MinorVersion = 1U;

			myServer->m_RequestCallBack(SocketPtr,RequestCls, ResponseCls);

			ResponseCls.Connection = RequestCls.Connection;
			ResponseCls.ContentEncoding.clear();
			if (ResponseCls.OriginalData.length() >= MinCompressMsgLength) {
				bool canBr = false, canGzip = false, canDeflate = false;
				for (auto i = RequestCls.AcceptEncoding.begin(); i != RequestCls.AcceptEncoding.end(); i++) {
					if ((*i).first == "br") {
						canBr = true;
					}
					else if ((*i).first == "gzip") {
						canGzip = true;
					}
					else if ((*i).first == "deflate") {
						canDeflate = true;
					}
				}

				if (canBr) {
					ResponseCls.ContentEncoding.push_back("br");
				}
				else if (canGzip) {
					ResponseCls.ContentEncoding.push_back("gzip");
				}
				else if (canDeflate) {
					ResponseCls.ContentEncoding.push_back("deflate");
				}
			}
			std::string ResponseString = ResponseCls.toResponseString();
			SocketPtr->SendMsg(ResponseString);
			
			if (EasyCrossPlatform::Parser::StringUtil::toLower(RequestCls.Connection) == "close") {
				SocketPtr->Disconnect();
			}

			return true;
		}
		else {
			SocketPtr->Disconnect();
			return false;
		}
	}
	else {
		return false;
	}
}

void EasyCrossPlatform::Network::HTTPServer::HTTPServer::SuperviseMaxConnTime(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	HTTPServer* myServer = (HTTPServer*)Parameters;
	while ((*RunningSign)) {
		if (!myServer->isListening()) {
			EasyCrossPlatform::Runtime::Chrono::sleepFor(10.0);
		}
		unsigned int SupervisedCount = 0U;
		float CurrentTime = (float)EasyCrossPlatform::Runtime::Chrono::accurateTimer();
		myServer->ConnectInfoMutex->lock();
		for (auto i = myServer->m_NormalHTTPConnInfo.begin(); i != myServer->m_NormalHTTPConnInfo.end(); i++) {
			if (CurrentTime - (*i).second.ConnectionStartTime >= myServer->m_MaxConnectionAliveTime && myServer->m_MaxConnectionAliveTime != 0.0F) {
				(*i).second.SocketPtr->Disconnect();
				SupervisedCount++;
			}
		}
		myServer->ConnectInfoMutex->unlock();
		if (myServer->m_MaxConnectionAliveTime >= 10.0F) {
			EasyCrossPlatform::Runtime::Chrono::sleepFor(12.0);
		}
		else {
			EasyCrossPlatform::Runtime::Chrono::sleepFor(5.0);
		}
		
	}
}