#include <XSYDTLSSocket.h>
//#include <iostream> For Debug Purpose

std::map<EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*, EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket*> EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::MyClassPtrs;
void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::setTrustedCAChain(const std::string & newCAChain)
{
	int ParseRst = mbedtls_x509_crt_parse(&this->m_sslCACert, (const unsigned char*) newCAChain.c_str(), newCAChain.length()+1);
	if (ParseRst<0) {
		this->onMbedTLSError(ParseRst);
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TCPConnectCallBack(bool Succeed, void * ClientSocketPtr)
{
	TLSAsyncClientSocket* myTLSClient = TLSAsyncClientSocket::MyClassPtrs[(TCPAsyncClientSocket*) ClientSocketPtr];
	if (Succeed) {
		mbedtls_debug_set_threshold(EasyCrossPlatform::Network::Socket::MBEDTLS_DEBUGLEVEL);
		mbedtls_ssl_config_defaults(&myTLSClient->m_sslConf,MBEDTLS_SSL_IS_CLIENT,MBEDTLS_SSL_TRANSPORT_STREAM,MBEDTLS_SSL_PRESET_DEFAULT);
		if (myTLSClient->VerifyServerCert) {
			mbedtls_ssl_conf_ca_chain(&myTLSClient->m_sslConf, &myTLSClient->m_sslCACert, NULL);
			mbedtls_ssl_conf_authmode(&myTLSClient->m_sslConf, MBEDTLS_SSL_VERIFY_REQUIRED);
		} else {
			mbedtls_ssl_conf_authmode(&myTLSClient->m_sslConf, MBEDTLS_SSL_VERIFY_NONE);
		}
		mbedtls_ssl_conf_rng(&myTLSClient->m_sslConf, mbedtls_ctr_drbg_random, &myTLSClient->m_sslCtr_drbg);
		mbedtls_ssl_conf_dbg(&myTLSClient->m_sslConf, TLSAsyncClientSocket::TLSDebugCallBack, myTLSClient);
		mbedtls_ssl_conf_min_version(&myTLSClient->m_sslConf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_2);
		mbedtls_ssl_conf_max_version(&myTLSClient->m_sslConf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3);
		mbedtls_ssl_setup(&myTLSClient->m_sslContext, &myTLSClient->m_sslConf);
		mbedtls_ssl_set_hostname(&myTLSClient->m_sslContext, myTLSClient->serverHostName.c_str());
		mbedtls_ssl_set_bio(&myTLSClient->m_sslContext, myTLSClient, TLSAsyncClientSocket::TLSSendCallback, TLSAsyncClientSocket::TLSRecvCallBack, NULL);
		myTLSClient->myWorkCls.setWork(TLSAsyncClientSocket::CompleteShakeProgress);
		myTLSClient->myWorkCls.StartJob(NULL,myTLSClient);
	}
	else {
		myTLSClient->onConnected(false);
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TCPDisconnectCallBack(void * ClientSocketPtr)
{
	TLSAsyncClientSocket* myTLSClient = TLSAsyncClientSocket::MyClassPtrs[(TCPAsyncClientSocket*)ClientSocketPtr];
	myTLSClient->onDisconnected();
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TCPMsgCallBack(const std::vector<byte> & Data, void * ClientSocketPtr)
{
	TLSAsyncClientSocket* myTLSClient = TLSAsyncClientSocket::MyClassPtrs[(TCPAsyncClientSocket*)ClientSocketPtr];
	myTLSClient->ReadTCPMsgMutex.lock();
	for (size_t i = 0U; i < Data.size(); i++) {
		myTLSClient->m_MsgWaitingForRead.push_back(Data[i]);
	}
	myTLSClient->ReadTCPMsgMutex.unlock();
	if (myTLSClient->m_Shaked) {
		//Because New Data has been added. Call for read
		byte myBuf[25565] = "";
		int ReadRst = mbedtls_ssl_read(&myTLSClient->m_sslContext, (unsigned char*)myBuf, 25565U);
		if (ReadRst == MBEDTLS_ERR_SSL_WANT_READ || ReadRst == MBEDTLS_ERR_SSL_WANT_WRITE) {
			return;
		}
		else if (ReadRst == MBEDTLS_ERR_SSL_CONN_EOF || ReadRst == 0 || ReadRst == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
			myTLSClient->Disconnect();
			return;
		}
		else if (ReadRst < 0) {
			myTLSClient->onMbedTLSError(ReadRst);
			myTLSClient->Disconnect();
			return;
		}
		std::vector<byte> realBuf;
		for (int i = 0; i < ReadRst; i++) {
			realBuf.push_back(myBuf[i]);
		}
		myTLSClient->onMsgCB(realBuf);
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TCPErrorCallBack(int ErrNo, const std::string & ErrDescription, void * ClientSocketPtr)
{
	TLSAsyncClientSocket* myTLSClient = TLSAsyncClientSocket::MyClassPtrs[(TCPAsyncClientSocket*)ClientSocketPtr];
	myTLSClient->onErrorOccured(ErrNo, ErrDescription);
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TLSDebugCallBack(void * TLSClientPtr, int level, const char * file, int line, const char * str)
{
	TLSAsyncClientSocket* myTLSClient = (TLSAsyncClientSocket*) TLSClientPtr;
	//std::cout << file << "[" << std::to_string(line) << "]:" << str << std::endl;
	//Debug Function, nothing to do.
}

int EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TLSRecvCallBack(void * TLSClientPtr, unsigned char * buf, size_t len)
{
	TLSAsyncClientSocket* myTLSClient = (TLSAsyncClientSocket*)TLSClientPtr;
	if (myTLSClient->m_MsgWaitingForRead.empty()) {
		return MBEDTLS_ERR_SSL_WANT_READ;
	}
	else {
		myTLSClient->ReadTCPMsgMutex.lock();
		size_t LengthToWrite = len > myTLSClient->m_MsgWaitingForRead.size() ? myTLSClient->m_MsgWaitingForRead.size() : len;
		
		for (size_t i = 0U; i < LengthToWrite; i++) {
			buf[i] = myTLSClient->m_MsgWaitingForRead.front();
			myTLSClient->m_MsgWaitingForRead.pop_front();
		}
		myTLSClient->ReadTCPMsgMutex.unlock();
		return static_cast<int>(LengthToWrite);
	}
}

int EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TLSSendCallback(void * TLSClientPtr, const unsigned char * buf, size_t len)
{
	TLSAsyncClientSocket* myTLSClient = (TLSAsyncClientSocket*)TLSClientPtr;
	std::vector<byte> myMsgToSend;
	for (size_t i = 0U; i < len; i++) {
		myMsgToSend.push_back(buf[i]);
	}
	myTLSClient->SendTCPMsgMutex.lock();
	myTLSClient->m_ClientSocket.SendMsg(myMsgToSend);
	myTLSClient->SendTCPMsgMutex.unlock();
	return static_cast<int>(len);
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::CompleteShakeProgress(std::thread::id myThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	TLSAsyncClientSocket* myTLSClient = (TLSAsyncClientSocket*)Parameters;
	if (!myTLSClient->m_Shaked) {
		int HandshakeRst = -1;
		while (HandshakeRst != 0 && (*RunningSign)) {
			HandshakeRst = mbedtls_ssl_handshake(&myTLSClient->m_sslContext);

			if (HandshakeRst == 0) {
				myTLSClient->onConnected(true);
				myTLSClient->m_Shaked = true;
				return;
			}
			else if(HandshakeRst != MBEDTLS_ERR_SSL_WANT_READ && HandshakeRst != MBEDTLS_ERR_SSL_WANT_WRITE) {
				myTLSClient->onConnected(false);
				myTLSClient->onMbedTLSError(HandshakeRst);
				myTLSClient->m_ClientSocket.Disconnect();
				return;
			}
		}
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::onDisconnected()
{
	mbedtls_ssl_session_reset(&this->m_sslContext);
	if (this->DisconnectCallBack != NULL) {
		this->DisconnectCallBack(this);
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::onConnected(bool Succeeded)
{
	if (this->ConnectCallBack != NULL) {
		this->ConnectCallBack(Succeeded, this);
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::onMsgCB(const std::vector<byte>& Data)
{
	if (this->MsgCallBack != NULL) {
		this->MsgCallBack(Data, this);
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::onErrorOccured(int errNo, const std::string & ErrDescription)
{
	if (this->ErrorCallBack != NULL) {
		this->ErrorCallBack(errNo, ErrDescription,this);
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::onMbedTLSError(int mbedErrNo)
{
	char ErrorMsg[200];
	mbedtls_strerror(mbedErrNo, ErrorMsg, 200);
	this->onErrorOccured(mbedErrNo, std::string(ErrorMsg));
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::setWorker(SocketWorker * newWorker)
{
	this->m_ClientSocket.setWorker(newWorker);
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::setRemoteIPAddr(const IpAddr & newIP)
{
	this->m_ClientSocket.setRemoteIPAddr(newIP);
}

EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::getMyIpAddr()
{
	return this->m_ClientSocket.getMyIpAddr();
}

EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::getRemoteAddr()
{
	return this->m_ClientSocket.getRemoteAddr();
}

EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TLSAsyncClientSocket()
{
	
}

EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TLSAsyncClientSocket(TLSAsyncClientSocket & oldClient)
{
	throw std::runtime_error("You cannot have a copy of this class");
	/* this->ConnectCallBack = oldClient.ConnectCallBack;
	this->DisconnectCallBack = oldClient.DisconnectCallBack;
	this->ErrorCallBack = oldClient.ErrorCallBack;
	this->MsgCallBack = oldClient.MsgCallBack;
	this->myWorkCls.setWork(TLSAsyncClientSocket::CompleteShakeProgress);
	this->m_ClientSocket = oldClient.m_ClientSocket;
	this->m_Inited = oldClient.m_Inited;
	this->m_MsgWaitingForRead = oldClient.m_MsgWaitingForRead;
	this->m_Shaked = oldClient.m_Shaked;
	this->m_sslCACert = oldClient.m_sslCACert;
	this->m_sslConf = oldClient.m_sslConf;
	this->m_sslContext = oldClient.m_sslContext;
	this->m_sslCtr_drbg = oldClient.m_sslCtr_drbg;
	this->m_sslEntropy = oldClient.m_sslEntropy;
	this->serverHostName = oldClient.serverHostName;
	this->VerifyServerCert = oldClient.VerifyServerCert;
	*/
	
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::Init()
{
	if (this->m_Inited) {
		return;
	}
	this->m_Inited = true;
	mbedtls_ssl_init(&this->m_sslContext);
	mbedtls_ssl_config_init(&this->m_sslConf);
	mbedtls_x509_crt_init(&m_sslCACert);
	mbedtls_ctr_drbg_init(&m_sslCtr_drbg);
	mbedtls_entropy_init(&this->m_sslEntropy);
	mbedtls_ctr_drbg_seed(&this->m_sslCtr_drbg, mbedtls_entropy_func, &this->m_sslEntropy, NULL, 0U);
	this->MyClassPtrs[&this->m_ClientSocket] = this;
	this->m_ClientSocket.Init();
	this->m_ClientSocket.ConnectCallBack = TLSAsyncClientSocket::TCPConnectCallBack;
	this->m_ClientSocket.DisconnectCallBack = TLSAsyncClientSocket::TCPDisconnectCallBack;
	this->m_ClientSocket.MsgCallBack = TLSAsyncClientSocket::TCPMsgCallBack;
	this->m_ClientSocket.ErrorCallBack = TLSAsyncClientSocket::TCPErrorCallBack;
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::Connect()
{
	this->m_ClientSocket.Connect();
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::Disconnect()
{
	this->m_ClientSocket.Disconnect();
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::SendMsg(const std::string & Data)
{
	size_t AmountWritten = 0U;
	int tmpInt = 0;
	std::string tmpData = "";
	while (AmountWritten != static_cast<int>(Data.length())) {
		tmpData = Data.substr(AmountWritten, Data.length() - AmountWritten);
		tmpInt = mbedtls_ssl_write(&this->m_sslContext, (const unsigned char*)tmpData.c_str(), Data.length()-AmountWritten);
		if (tmpInt > 0) {
			AmountWritten += tmpInt;
		}
		else {
			this->Disconnect();
			return;
		}
	}
	return;
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::SendMsg(const std::vector<byte>& Data)
{
	this->SendMsg(EasyCrossPlatform::Parser::StringUtil::fromBytes(Data));
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::Destroy()
{
	if (!this->m_Inited) {
		return;
	}
	this->m_Inited = false;
	this->Disconnect();
	this->m_ClientSocket.Destroy();
	this->myWorkCls.StopJob();
	mbedtls_ssl_free(&this->m_sslContext);
	mbedtls_ssl_config_free(&this->m_sslConf);
	mbedtls_ctr_drbg_free(&this->m_sslCtr_drbg);
	mbedtls_entropy_free(&this->m_sslEntropy);
	mbedtls_x509_crt_free(&this->m_sslCACert);
	this->MyClassPtrs.erase(&this->m_ClientSocket);
}

EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::~TLSAsyncClientSocket()
{
	this->Destroy();
}
