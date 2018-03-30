#include <XSYDRequest.h>

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TCPMsg_CB(const std::vector<byte>& Msg, void * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->CustomData;
	std::string newMsg = EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg);
	myRequestCls->m_MsgWaitingForRead += newMsg;
	myRequestCls->m_IsProcessing = false;
	myRequestCls->m_operationSucceed = true;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TCPConn_CB(bool Succeed, void * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->CustomData;
	if (!Succeed) {
		myRequestCls->m_operationSucceed = false;
	}
	else {
		myRequestCls->m_operationSucceed = true;
	}
	myRequestCls->m_IsProcessing = false;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TCPDisConn_CB(void * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->CustomData;
	myRequestCls->m_operationSucceed = false;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TCPErr_CB(int ErrNo, const std::string & ErrDescription, void * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->CustomData;
	myRequestCls->m_operationSucceed = false;
	
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TLSMsg_CB(const std::vector<byte>& Msg, void * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->CustomData;
	std::string newMsg = EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg);
	myRequestCls->m_MsgWaitingForRead += newMsg;
	myRequestCls->m_IsProcessing = false;
	myRequestCls->m_operationSucceed = true;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TLSConn_CB(bool Succeed, void * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->CustomData;
	if (!Succeed) {
		myRequestCls->m_operationSucceed = false;
	}
	else {
		myRequestCls->m_operationSucceed = true;
	}
	myRequestCls->m_IsProcessing = false;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TLSDisConn_CB(void * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->CustomData;
	myRequestCls->m_operationSucceed = false;
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::m_TLSErr_CB(int ErrNo, const std::string & ErrDescription, void * ClassPtr)
{
	EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket* myClientSocket = (EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket*) ClassPtr;
	WebsiteRequest* myRequestCls = (WebsiteRequest*)myClientSocket->CustomData;
	myRequestCls->m_operationSucceed = false;
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
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::cleanRequest()
{
	this->m_ResquestContent.cleanUp();
}

void EasyCrossPlatform::Network::Request::WebsiteRequest::performRequest()
{
	this->cleanUp();
	this->m_ResquestContent.RequestedURL.fromURLString(this->RequestURL);
	unsigned int mRequestPort = 0U;
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
	this->m_ResquestContent.FieldsValues["Connection"] = "close";
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
		this->m_TLSClientSocket.setWorker(&this->m_SharedSocketWorker);
		this->m_TLSClientSocket.ConnectCallBack = this->m_TLSConn_CB;
		this->m_TLSClientSocket.DisconnectCallBack = this->m_TLSDisConn_CB;
		this->m_TLSClientSocket.ErrorCallBack = this->m_TLSErr_CB;
		this->m_TLSClientSocket.MsgCallBack = this->m_TLSMsg_CB;
		this->m_TLSClientSocket.VerifyServerCert = this->VerifyHTTPSCert;
		this->m_TLSClientSocket.Init();
		this->m_TLSClientSocket.setTrustedCAChain(std::string(EasyCrossPlatform::Network::Request::TrustedCA, EasyCrossPlatform::Network::Request::TrustedCA_length));
		this->m_TLSClientSocket.setRemoteIPAddr(DecodedIP[0]);
		this->m_TLSClientSocket.serverHostName = mRequestDomain;
		this->m_TLSClientSocket.CustomData = this;
		this->m_IsProcessing = true;
		this->m_TLSClientSocket.Connect();
		while (this->m_IsProcessing) {
			EasyCrossPlatform::Runtime::Chrono::sleepFor(0.05);
		}
		if (!this->m_operationSucceed) {
			this->SucceedRequest = false;
			this->m_TLSClientSocket.Destroy();
			return;
		}
		
		this->m_TLSClientSocket.SendMsg(mRequestContent);
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
			this->m_TLSClientSocket.Destroy();
			return;
		}
		this->m_TLSClientSocket.Disconnect();
		this->m_TLSClientSocket.Destroy();
		this->SucceedRequest = true;
		this->ResponseOriginalData = this->m_MsgWaitingForRead;
	}
	else {
		this->m_ClientSocket.setWorker(&this->m_SharedSocketWorker);
		this->m_ClientSocket.ConnectCallBack = this->m_TCPConn_CB;
		this->m_ClientSocket.DisconnectCallBack = this->m_TCPDisConn_CB;
		this->m_ClientSocket.ErrorCallBack = this->m_TCPErr_CB;
		this->m_ClientSocket.MsgCallBack = this->m_TCPMsg_CB;
		this->m_ClientSocket.Init();
		this->m_ClientSocket.setRemoteIPAddr(DecodedIP[0]);
		this->m_ClientSocket.CustomData = this;
		this->m_IsProcessing = true;
		this->m_ClientSocket.Connect();
		while (this->m_IsProcessing) {
			EasyCrossPlatform::Runtime::Chrono::sleepFor(0.05);
		}
		if (!this->m_operationSucceed) {
			this->SucceedRequest = false;
			this->m_ClientSocket.Destroy();
			return;
		}

		this->m_ClientSocket.SendMsg(mRequestContent);

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
			this->m_ClientSocket.Destroy();
			return;
		}
		this->m_ClientSocket.Disconnect();
		this->m_ClientSocket.Destroy();
		this->SucceedRequest = true;
		this->ResponseOriginalData = this->m_MsgWaitingForRead;
	}
	this->m_MsgWaitingForRead = "";
	return;
}

std::vector<std::pair<std::string, float>> EasyCrossPlatform::Network::Request::generateAcceptEncodingList()
{
	std::vector<std::pair<std::string, float>> tmpArray;
	
	for (unsigned int i = 0U; i < acceptEncodingNum; i++) {
		tmpArray.push_back(std::pair<std::string, float>(acceptEncodings[i], acceptEncodingWeights[i]));
	}
	return tmpArray;
}