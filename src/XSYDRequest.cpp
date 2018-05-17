#include <XSYDRequest.h>

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TCPMsg_CB(const std::vector<byte>& Msg, EasyCrossPlatform::Network::Socket::StandardClientSocket * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->getCustomData();
	std::string newMsg = EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg);
	myRequestCls->m_MsgWaitingForRead += newMsg;
	myRequestCls->m_IsProcessing = false;
	myRequestCls->m_operationSucceed = true;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TCPConn_CB(bool Succeed, EasyCrossPlatform::Network::Socket::StandardClientSocket * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->getCustomData();
	if (!Succeed) {
		myRequestCls->m_operationSucceed = false;
	}
	else {
		myRequestCls->m_operationSucceed = true;
	}
	myRequestCls->m_IsProcessing = false;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TCPDisConn_CB(EasyCrossPlatform::Network::Socket::StandardClientSocket * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->getCustomData();
	myRequestCls->m_operationSucceed = false;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TCPErr_CB(int ErrNo, const std::string & ErrDescription, EasyCrossPlatform::Network::Socket::StandardClientSocket * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->getCustomData();
	myRequestCls->m_operationSucceed = false;
	
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TLSMsg_CB(const std::vector<byte>& Msg, EasyCrossPlatform::Network::Socket::StandardClientSocket * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket* myClientSocket =  (EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->getCustomData();
	std::string newMsg = EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg);
	myRequestCls->m_MsgWaitingForRead += newMsg;
	myRequestCls->m_IsProcessing = false;
	myRequestCls->m_operationSucceed = true;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TLSConn_CB(bool Succeed, EasyCrossPlatform::Network::Socket::StandardClientSocket * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->getCustomData();
	if (!Succeed) {
		myRequestCls->m_operationSucceed = false;
	}
	else {
		myRequestCls->m_operationSucceed = true;
	}
	myRequestCls->m_IsProcessing = false;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TLSDisConn_CB(EasyCrossPlatform::Network::Socket::StandardClientSocket * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->getCustomData();
	myRequestCls->m_operationSucceed = false;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TLSErr_CB(int ErrNo, const std::string & ErrDescription, EasyCrossPlatform::Network::Socket::StandardClientSocket * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->getCustomData();
	myRequestCls->m_operationSucceed = false;
}

EasyCrossPlatform::Network::Request::WebsiteRequest::WebsiteRequest()
{
	this->cleanUp();
	this->cleanRequest();
}

EasyCrossPlatform::Network::Request::WebsiteRequest::WebsiteRequest(WebsiteRequest & OldRequest)
{
	this->Method = OldRequest.Method;
	this->m_IsHTTPS = OldRequest.m_IsHTTPS;
	this->m_IsProcessing = false;
	this->m_MsgWaitingForRead.clear();
	this->m_operationSucceed = OldRequest.m_operationSucceed;
	this->m_ResquestContent = OldRequest.m_ResquestContent;
	this->RequestData = OldRequest.RequestData;
	this->RequestURL = OldRequest.RequestURL;
	this->ResponseContent = OldRequest.ResponseContent;
	this->ResponseOriginalData = OldRequest.ResponseOriginalData;
	this->SucceedRequest = OldRequest.SucceedRequest;
	this->VerifyHTTPSCert = OldRequest.VerifyHTTPSCert;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::cleanUp()
{
	//Clean the whole class up!
	this->m_IsProcessing = false;
	this->m_operationSucceed = true;
	this->m_IsHTTPS = false;
	this->m_MsgWaitingForRead = "";
	this->ResponseContent.cleanUp();
	this->m_ResquestContent.MajorVersion = 1U;
	this->m_ResquestContent.MinorVersion = 1U;
	if (this->m_ClientSocket != NULL) {
		delete this->m_ClientSocket;
		this->m_ClientSocket = NULL;
	}
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::cleanRequest()
{
	this->m_ResquestContent.cleanUp();
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::performRequest()
{
	this->cleanUp();
	this->m_ResquestContent.RequestedURL.fromURLString(this->RequestURL);
	unsigned short mRequestPort = 0U;
	std::string tmpProtoStr = EasyCrossPlatform::Parser::StringUtil::toLower(m_ResquestContent.RequestedURL.Protocol);
	if (tmpProtoStr.empty() || tmpProtoStr == "http") {
		this->m_IsHTTPS = false;
		if (this->m_ResquestContent.RequestedURL.Port != 0U) {
			mRequestPort = this->m_ResquestContent.RequestedURL.Port;
		}
		else {
			mRequestPort = 80U;
		}
	}
	else if(tmpProtoStr == "https") {
		this->m_IsHTTPS = true;
		if (this->m_ResquestContent.RequestedURL.Port != 0U) {
			mRequestPort = this->m_ResquestContent.RequestedURL.Port;
		}
		else {
			mRequestPort = 443U;
		}
	}
	else {
		throw std::invalid_argument("Protocol not recognized");
	}
	this->m_ResquestContent.AcceptEncoding = Request::generateAcceptEncodingList();
	this->m_ResquestContent.SetFieldWithSingleValue("Connection", "close");
	this->m_ResquestContent.OriginalData = this->RequestData;
	this->m_ResquestContent.Method = std::to_string(this->Method);
	std::string mRequestContent = this->m_ResquestContent.toReqString();
	std::string mRequestDomain = this->m_ResquestContent.RequestedURL.Host;
	
	std::vector<EasyCrossPlatform::Network::Socket::IpAddr> DecodedIP;
	this->m_DNSRequest.Init();
	DecodedIP = this->m_DNSRequest.getDomainAddr_IPv4v6(mRequestDomain, mRequestPort);
	this->m_DNSRequest.Destroy();
	if (DecodedIP.empty()) {
		this->SucceedRequest = false;
		return;
	}
	if (this->m_IsHTTPS) {
		EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket* m_TLSClientSocket = new EasyCrossPlatform::Network::Socket::TLS::TLSAsyncClientSocket(&this->m_SharedSocketWorker, DecodedIP[0], this->VerifyHTTPSCert, mRequestDomain);
		this->m_ClientSocket = m_TLSClientSocket;
		
		this->m_ClientSocket->SetConnectCallBack(this->m_TLSConn_CB);
		this->m_ClientSocket->SetDisconnectCallBack(this->m_TLSDisConn_CB);
		this->m_ClientSocket->SetErrorCallBack(this->m_TLSErr_CB);
		this->m_ClientSocket->SetMsgCallBack(this->m_TLSMsg_CB);
		m_TLSClientSocket->setTrustedCAChain(std::string(EasyCrossPlatform::Network::Request::TrustedCA, EasyCrossPlatform::Network::Request::TrustedCA_length));
		m_TLSClientSocket->setCustomData(this);
		this->m_IsProcessing = true;
		this->m_ClientSocket->Connect();
		while (this->m_IsProcessing) {
			EasyCrossPlatform::Runtime::Chrono::sleepFor(0.05);
		}
		if (!this->m_operationSucceed) {
			this->SucceedRequest = false;
			this->m_ClientSocket->Disconnect();
			delete this->m_ClientSocket;
			this->m_ClientSocket = NULL;
			return;
		}
		
		this->m_ClientSocket->SendMsg(mRequestContent);
		//Begin Reading 
		this->m_IsProcessing = true;
		double LastRecvTick = 0.0;
		double CurrentTick = 0.0;
		
		EasyCrossPlatform::Parser::HTTP::HTTPParseReturnVal mParseResult;
		mParseResult.msgIsEnough = false;
		mParseResult.canDecode = false;
		mParseResult.msgIsHTTP = false;
		mParseResult.onError = false;
		mParseResult.RemainMsg = "";

		while (!mParseResult.msgIsEnough) {
			LastRecvTick = EasyCrossPlatform::Runtime::Chrono::accurateTimer();
			while (this->m_IsProcessing) {
				CurrentTick = EasyCrossPlatform::Runtime::Chrono::accurateTimer();
				if (CurrentTick - LastRecvTick > ReqTimeout) {
					this->m_operationSucceed = false;
					break;
				}
				EasyCrossPlatform::Runtime::Chrono::sleepFor(0.05);
			}
			mParseResult = this->ResponseContent.fromResponseString(this->m_MsgWaitingForRead);
			if (mParseResult.canDecode && mParseResult.msgIsEnough && mParseResult.msgIsHTTP & !mParseResult.onError) {
				if (this->ResponseContent.ResponseCode == 100U) { //100 Continue is useless!
					this->m_MsgWaitingForRead = mParseResult.RemainMsg;
					mParseResult.msgIsEnough = false;
				}
			}
			this->m_IsProcessing = true;
			if (this->m_operationSucceed == false) {
				break;
			}
		}
		this->m_IsProcessing = false;
		if (!mParseResult.canDecode || !mParseResult.msgIsEnough || !mParseResult.msgIsHTTP || mParseResult.onError) {
			this->SucceedRequest = false;
			this->m_ClientSocket->Disconnect();
			delete this->m_ClientSocket;
			this->m_ClientSocket = NULL;
			return;
		}
		this->m_ClientSocket->Disconnect();
		delete this->m_ClientSocket;
		this->m_ClientSocket = NULL;
		this->SucceedRequest = true;
		this->ResponseOriginalData = this->m_MsgWaitingForRead;
	}
	else {
		EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket* m_TCPSocket = new EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket(DecodedIP[0], &this->m_SharedSocketWorker, 0U);
		this->m_ClientSocket = m_TCPSocket;
		this->m_ClientSocket->SetConnectCallBack(this->m_TCPConn_CB);
		this->m_ClientSocket->SetDisconnectCallBack(this->m_TCPDisConn_CB);
		this->m_ClientSocket->SetErrorCallBack(this->m_TCPErr_CB);
		this->m_ClientSocket->SetMsgCallBack(this->m_TCPMsg_CB);
		m_TCPSocket->setCustomData(this);
		this->m_IsProcessing = true;
		this->m_ClientSocket->Connect();
		while (this->m_IsProcessing) {
			EasyCrossPlatform::Runtime::Chrono::sleepFor(0.05);
		}
		if (!this->m_operationSucceed) {
			this->SucceedRequest = false;
			this->m_ClientSocket->Disconnect();
			delete this->m_ClientSocket;
			this->m_ClientSocket = NULL;
			return;
		}

		this->m_ClientSocket->SendMsg(mRequestContent);

		//Begin Reading 
		this->m_IsProcessing = true;
		double LastRecvTick = 0.0;
		double CurrentTick = 0.0;

		EasyCrossPlatform::Parser::HTTP::HTTPParseReturnVal mParseResult;
		mParseResult.msgIsEnough = false;
		mParseResult.canDecode = false;
		mParseResult.msgIsHTTP = false;
		mParseResult.onError = false;
		mParseResult.RemainMsg = "";

		while (!mParseResult.msgIsEnough) {
			LastRecvTick = EasyCrossPlatform::Runtime::Chrono::accurateTimer();
			while (this->m_IsProcessing) {
				CurrentTick = EasyCrossPlatform::Runtime::Chrono::accurateTimer();
				if (CurrentTick - LastRecvTick > ReqTimeout) {
					this->m_operationSucceed = false;
					break;
				}
				EasyCrossPlatform::Runtime::Chrono::sleepFor(0.05);
			}
			mParseResult = this->ResponseContent.fromResponseString(this->m_MsgWaitingForRead);
			if (mParseResult.canDecode && mParseResult.msgIsEnough && mParseResult.msgIsHTTP & !mParseResult.onError) {
				if (this->ResponseContent.ResponseCode == 100U) { //100 Continue is useless!
					this->m_MsgWaitingForRead = mParseResult.RemainMsg;
					mParseResult.msgIsEnough = false;
				}
			}
			this->m_IsProcessing = true;
			if (this->m_operationSucceed == false) {
				break;
			}
		}
		this->m_IsProcessing = false;
		if (!mParseResult.canDecode || !mParseResult.msgIsEnough || !mParseResult.msgIsHTTP || mParseResult.onError) {
			this->SucceedRequest = false;
			this->m_ClientSocket->Disconnect();
			delete this->m_ClientSocket;
			this->m_ClientSocket = NULL;
			return;
		}
		this->m_ClientSocket->Disconnect();
		delete this->m_ClientSocket;
		this->m_ClientSocket = NULL;
		this->SucceedRequest = true;
		this->ResponseOriginalData = this->m_MsgWaitingForRead;
	}
	this->m_MsgWaitingForRead = "";
	return;
}

EasyCrossPlatform::Network::Request::WebsiteRequest::~WebsiteRequest()
{
	if (this->m_ClientSocket != NULL) {
		delete this->m_ClientSocket;
	}
}

std::vector<std::pair<std::string, float>> EasyCrossPlatform::Network::Request::generateAcceptEncodingList()
{
	std::vector<std::pair<std::string, float>> tmpArray;
	
	for (unsigned int i = 0U; i < acceptEncodingNum; i++) {
		tmpArray.push_back(std::pair<std::string, float>(acceptEncodings[i], acceptEncodingWeights[i]));
	}
	return tmpArray;
}