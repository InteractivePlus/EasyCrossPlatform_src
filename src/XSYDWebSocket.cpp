#include <XSYDWebSocket.h>

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::Connect()
{
	throw std::runtime_error("You cannot connect using this class");
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::Disconnect()
{
	if (this->m_Shaked) {
		//Make a closing frame
		unsigned char mCloseHolder[500];
		int NewPongLength = this->mParser.makeFrame(WebSocketFrameType::CLOSING_FRAME, reinterpret_cast<unsigned char*>(""), 0, mCloseHolder, 500);
		this->m_SocketHandle->SendMsg(std::string(reinterpret_cast<char*>(mCloseHolder), NewPongLength));
	}
	this->m_Shaked = false;
	this->m_SocketHandle->Disconnect();
}

bool EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::isConnected()
{
	return this->m_Shaked;
}

EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::~WebsocketSingleConnection()
{
	this->Destroy();
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::Init()
{
	this->m_SocketHandle->SetConnectCallBack(WebsocketSingleConnection::m_ConnectCallBack);
	this->m_SocketHandle->SetDisconnectCallBack(WebsocketSingleConnection::m_DisconnCallBack);
	this->m_SocketHandle->SetErrorCallBack(WebsocketSingleConnection::m_ErrCallBack);
	this->m_SocketHandle->SetMsgCallBack(WebsocketSingleConnection::m_MsgCallBack);
	this->m_SocketHandle->setCustomData(this);
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::Destroy()
{
	if (this->m_deleteSocketAfter) {
		if (this->m_SocketHandle != NULL) {
			delete this->m_SocketHandle;
			this->m_SocketHandle = NULL;
		}
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onConnect(bool Succeed)
{
	if (this->m_ConnectCallBack != NULL) {
		this->m_ConnectCallBack(Succeed, this);
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onMsg(const std::string & Msg)
{
	if (this->m_MsgCallBack != NULL) {
		this->m_MsgCallBack(EasyCrossPlatform::Parser::StringUtil::toBytes(Msg), this);
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onDisconn()
{
	this->m_Shaked = false;
	if (this->m_DisconnCallBack != NULL) {
		this->m_DisconnCallBack(this);
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onError(int errNo, const std::string & errDescription)
{
	if (this->m_ErrCallBack != NULL) {
		this->m_ErrCallBack(errNo, errDescription, this);
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onPong()
{
	if (this->PongFrameTime != 0.0) {
		double CurrentTime = EasyCrossPlatform::Runtime::Chrono::accurateTimer();
		double TimeDiff = CurrentTime - this->PongFrameTime;
		if (this->m_LatencyCallBack != NULL) {
			this->m_LatencyCallBack(TimeDiff, this);
		}
		this->PongFrameTime = 0.0;
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::m_Socket_Connection(bool Succeed, Socket::StandardClientSocket * SocketPtr)
{
	//This function never get called.
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::m_Socket_Msg(const std::vector<byte>& Msg, Socket::StandardClientSocket * SocketPtr)
{
	WebsocketSingleConnection* myWSClient = (WebsocketSingleConnection*)SocketPtr->getCustomData();
	for (byte i : Msg) {
		myWSClient->BufferWaitingForRead.push_back(i);
	}
	if (myWSClient->m_Shaked) {
		while (myWSClient->checkNewMsg());
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::m_Socket_Disconn(Socket::StandardClientSocket * SocketPtr)
{
	WebsocketSingleConnection* myWSClient = (WebsocketSingleConnection*)SocketPtr->getCustomData();
	myWSClient->onDisconn();
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::m_Socket_Error(int errNo, const std::string & errDescription, Socket::StandardClientSocket * SocketPtr)
{
	WebsocketSingleConnection* myWSClient = (WebsocketSingleConnection*)SocketPtr->getCustomData();
	myWSClient->onError(errNo, errDescription);
}

bool EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::checkNewMsg()
{
	unsigned int BufferLength = this->BufferWaitingForRead.size();
	unsigned char* WaitingReadBuffer = new unsigned char[BufferLength];
	unsigned char* DecodedMsgBuffer = new unsigned char[BufferLength - 16U];
	for (byte i = 0U; i < this->BufferWaitingForRead.size(); i++) {
		WaitingReadBuffer[i] = this->BufferWaitingForRead[i];
	}
	bool bResult = false;
	int NewLength = static_cast<int>(BufferLength);
	int OutLength = 0;
	WebSocketFrameType fType = this->mParser.getFrame(WaitingReadBuffer, NewLength, DecodedMsgBuffer, NewLength - 16, &OutLength);
	
	std::string StrResult;
	int NewPongLength = 0;
	switch (fType) {
	case WebSocketFrameType::BINARY_FRAME:
	case WebSocketFrameType::TEXT_FRAME:
		StrResult = std::string(reinterpret_cast<char*>(DecodedMsgBuffer), static_cast<unsigned int>(OutLength));
		this->onMsg(StrResult);
		bResult = true;
		break;
	case WebSocketFrameType::INCOMPLETE_BINARY_FRAME:
	case WebSocketFrameType::INCOMPLETE_FRAME:
	case WebSocketFrameType::INCOMPLETE_TEXT_FRAME:
		bResult = false;
		break;
	case WebSocketFrameType::PING_FRAME:
		//Make a PONG Frame
		unsigned char mPongHolder[500];
		NewPongLength = this->mParser.makeFrame(WebSocketFrameType::PONG_FRAME, reinterpret_cast<unsigned char*>(""), 0, mPongHolder, 500);
		this->m_SocketHandle->SendMsg(std::string(reinterpret_cast<char*>(mPongHolder), NewPongLength));
		bResult = true;
		break;
	case WebSocketFrameType::PONG_FRAME:
		//Client responds!
		this->onPong();
		bResult = true;
		break;
	case WebSocketFrameType::CLOSING_FRAME:
		//Close the connection!
		this->m_Shaked = false;
		this->Disconnect();
		break;
	case WebSocketFrameType::OPENING_FRAME:
		bResult = true;
		break;
	default:
		bResult = false;
		this->onError(-1, "Websocket Parse Error");
		this->Disconnect();
		break;
	}
	if (bResult) {
		for (int i = 0; i < NewLength; i++) {
			//Delete all used inputs
			this->BufferWaitingForRead.pop_front();
		}
	}
	if (this->BufferWaitingForRead.empty()) {
		bResult = false;
	}
	return bResult;
}

EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::WebsocketSingleConnection(const std::string & HandshakeRequest, Socket::StandardClientSocket * ClientSocket, bool DeleteSocketAfter)
{
	this->m_SocketHandle = ClientSocket;
	this->m_deleteSocketAfter = DeleteSocketAfter;

	//HandShake!
	unsigned char* mHandshakeMsg = new unsigned char[HandshakeRequest.length()];
	memcpy(mHandshakeMsg, HandshakeRequest.data(), HandshakeRequest.size());
	mParser.parseHandshake(mHandshakeMsg, HandshakeRequest.size());
	this->m_SocketHandle->SendMsg(mParser.answerHandshake());
	this->m_Shaked = true;
	this->Init();
	this->onConnect(true);
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetConnectCallBack(Socket::StandardClientConnCallBack mCB)
{
	this->m_ConnectCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetMsgCallBack(Socket::StandardClientMsgCallBack mCB)
{
	this->m_MsgCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetDisconnectCallBack(Socket::StandardClientDisconnectCallBack mCB)
{
	this->m_DisconnCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetErrorCallBack(Socket::StandardClientErrorCallBack mCB)
{
	this->m_ErrCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetLatencyTestCallBack(WebsocketLatencyTestCallBack mCB)
{
	this->m_LatencyCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::TestLatency()
{
	if (!this->m_Shaked) {
		throw std::runtime_error("You cannot test latency while the connection is closed");
	}
	//Send a Ping Frame!
	unsigned char mPingHolder[500];
	int NewPongLength = this->mParser.makeFrame(WebSocketFrameType::PING_FRAME, reinterpret_cast<unsigned char*>(""), 0, mPingHolder, 500);
	this->m_SocketHandle->SendMsg(std::string(reinterpret_cast<char*>(mPingHolder), NewPongLength));
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SendMsg(const std::string & Msg)
{
	if (!this->m_Shaked) {
		throw std::runtime_error("You cannot send an msg while the connection is closed");
	}
	unsigned int MsgLength = Msg.length();
	unsigned char* mMsgArr = new unsigned char[MsgLength];
	unsigned char* mEncodedArr = new unsigned char[MsgLength];
	memcpy(mMsgArr, Msg.data(), MsgLength);
	int NewMsgLength = this->mParser.makeFrame(WebSocketFrameType::BINARY_FRAME, mMsgArr, static_cast<int>(MsgLength), mEncodedArr, MsgLength);
	std::string StrToSend = std::string(reinterpret_cast<char*>(mEncodedArr), static_cast<unsigned int>(NewMsgLength));
	try {
		this->m_SocketHandle->SendMsg(StrToSend);
	}
	catch (runtime_error e) {
		throw e;
	}
	delete[] mMsgArr;
	delete[] mEncodedArr;
	return;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SendMsg(const std::vector<byte>& Msg)
{
	try {
		this->SendMsg(EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg));
	}
	catch (std::runtime_error e) {
		throw e;
	}
}
