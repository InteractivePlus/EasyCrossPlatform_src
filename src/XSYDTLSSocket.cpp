#include <XSYDTLSSocket.h>
#include <iostream> //For Debug Purpose


void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::setTrustedCAChain(const std::string & newCAChain)
{
	int ParseRst = mbedtls_x509_crt_parse(&this->m_sslCACert, (const unsigned char*) newCAChain.c_str(), newCAChain.length()+1U);
	if (ParseRst<0) {
		this->onMbedTLSError(ParseRst);
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TCPConnectCallBack(bool Succeed, void * ClientSocketPtr)
{
	int ret = 0;
	TLSAsyncClientSocket* myTLSClient = (TLSAsyncClientSocket*) ((TCPAsyncClientSocket*) ClientSocketPtr)->CustomData;
	if (Succeed) {
		mbedtls_ssl_config_defaults(&myTLSClient->m_sslConf,MBEDTLS_SSL_IS_CLIENT,MBEDTLS_SSL_TRANSPORT_STREAM,MBEDTLS_SSL_PRESET_DEFAULT);
		if (myTLSClient->VerifyServerCert) {
			mbedtls_ssl_conf_ca_chain(&myTLSClient->m_sslConf, &myTLSClient->m_sslCACert, NULL);
			mbedtls_ssl_conf_authmode(&myTLSClient->m_sslConf, MBEDTLS_SSL_VERIFY_REQUIRED);
		} else {
			mbedtls_ssl_conf_authmode(&myTLSClient->m_sslConf, MBEDTLS_SSL_VERIFY_NONE);
		}
		if (!myTLSClient->SupportedALPNProtocols.empty()) {
			myTLSClient->m_TMPALPNProtoNum = myTLSClient->SupportedALPNProtocols.size();
			myTLSClient->m_TMPALPNProtoList = new char*[myTLSClient->m_TMPALPNProtoNum+1];
			myTLSClient->m_TMPALPNProtoList[myTLSClient->m_TMPALPNProtoNum] = NULL;
			for (unsigned int i = 0; i < myTLSClient->m_TMPALPNProtoNum; i++) {
				myTLSClient->m_TMPALPNProtoList[i] = new char[myTLSClient->SupportedALPNProtocols[i].size() + 1];
				for (unsigned int StringIt = 0U; StringIt < myTLSClient->SupportedALPNProtocols[i].size(); StringIt++) {
					myTLSClient->m_TMPALPNProtoList[i][StringIt] = myTLSClient->SupportedALPNProtocols[i][StringIt];
				}
				myTLSClient->m_TMPALPNProtoList[i][myTLSClient->SupportedALPNProtocols[i].size()] = '\0';
			}
			mbedtls_ssl_conf_alpn_protocols(&myTLSClient->m_sslConf,(const char**) myTLSClient->m_TMPALPNProtoList);
		}
		mbedtls_ssl_conf_handshake_timeout(&myTLSClient->m_sslConf, myTLSClient->MinHandshakeTime, myTLSClient->MaxHandshakeTime);
		mbedtls_ssl_conf_rng(&myTLSClient->m_sslConf, mbedtls_ctr_drbg_random, &myTLSClient->m_sslCtr_drbg);
		mbedtls_ssl_conf_dbg(&myTLSClient->m_sslConf, TLSAsyncClientSocket::TLSDebugCallBack, myTLSClient);
		mbedtls_ssl_conf_min_version(&myTLSClient->m_sslConf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_2);
		mbedtls_ssl_conf_max_version(&myTLSClient->m_sslConf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3);
		mbedtls_ssl_setup(&myTLSClient->m_sslContext, &myTLSClient->m_sslConf);
		if (!myTLSClient->serverHostName.empty()) {
			mbedtls_ssl_set_hostname(&myTLSClient->m_sslContext, myTLSClient->serverHostName.c_str());
		}
		mbedtls_ssl_set_bio(&myTLSClient->m_sslContext, myTLSClient, TLSAsyncClientSocket::TLSSendCallback, TLSAsyncClientSocket::TLSRecvCallBack, NULL);
		myTLSClient->myWorkCls.setWork(TLSAsyncClientSocket::CompleteShakeProgress);
		myTLSClient->myWorkCls.StartJob(NULL,myTLSClient);
	}else{
		myTLSClient->onConnected(false);
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TCPDisconnectCallBack(void * ClientSocketPtr)
{
	TLSAsyncClientSocket* myTLSClient = (TLSAsyncClientSocket*) ((TCPAsyncClientSocket*) ClientSocketPtr)->CustomData;
	myTLSClient->onDisconnected();
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TCPMsgCallBack(const std::vector<byte> & Data, void * ClientSocketPtr)
{
	TLSAsyncClientSocket* myTLSClient = (TLSAsyncClientSocket*) ((TCPAsyncClientSocket*)ClientSocketPtr)->CustomData;
	myTLSClient->ReadTCPMsgMutex.lock();
	for (size_t i = 0U; i < Data.size(); i++) {
		myTLSClient->m_MsgWaitingForRead.push_back(Data[i]);
	}
	myTLSClient->ReadTCPMsgMutex.unlock();
	while (myTLSClient->CheckMsg() == true) {
		//Checking Msg!
	}
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TCPErrorCallBack(int ErrNo, const std::string & ErrDescription, void * ClientSocketPtr)
{
	TLSAsyncClientSocket* myTLSClient = (TLSAsyncClientSocket*)((TCPAsyncClientSocket*)ClientSocketPtr)->CustomData;
	myTLSClient->onErrorOccured(ErrNo, ErrDescription);
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::TLSDebugCallBack(void * TLSClientPtr, int level, const char * file, int line, const char * str)
{
	TLSAsyncClientSocket* myTLSClient = (TLSAsyncClientSocket*) TLSClientPtr;
	std::cout << file << "[" << std::to_string(line) << "]:" << str << std::endl;
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
				const char* NegotiatedALPNProtol = mbedtls_ssl_get_alpn_protocol(&myTLSClient->m_sslContext);
				if (NegotiatedALPNProtol != NULL) {
					myTLSClient->NegotiatedALPNProtocols.assign(NegotiatedALPNProtol);
				}
				myTLSClient->onConnected(true);
				myTLSClient->m_Shaked = true;
				if (!myTLSClient->m_MsgWaitingForRead.empty()) {
					while (myTLSClient->CheckMsg() == true) {
						//Checking New Msg!
					}
				}
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
	this->myWorkCls.StopJob();
	if (!this->m_MsgWaitingForRead.empty()) {
		while (this->CheckMsg() == true) {
			//Checking Msg!
		}
	}
	this->m_Shaked = false;
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

bool EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::CheckMsg()
{
	if (this->m_Shaked) {
		if (this->m_MsgWaitingForRead.empty()) {
			return false;
		}
		//Because New Data has been added. Call for read
		byte myBuf[25565] = "";
		int ReadRst = mbedtls_ssl_read(&this->m_sslContext, (unsigned char*)myBuf, 25565U);
		if (ReadRst == MBEDTLS_ERR_SSL_WANT_READ || ReadRst == MBEDTLS_ERR_SSL_WANT_WRITE) {
			return false;
		}
		else if (ReadRst == MBEDTLS_ERR_SSL_CONN_EOF || ReadRst == 0 || ReadRst == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
			this->Disconnect();
			return false;
		}
		else if (ReadRst < 0) {
			this->onMbedTLSError(ReadRst);
			this->Disconnect();
			return false;
		}
		std::vector<byte> realBuf;
		for (int i = 0; i < ReadRst; i++) {
			realBuf.push_back(myBuf[i]);
		}
		this->onMsgCB(realBuf);
		return true;
	}
	return false;
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
	this->m_MsgWaitingForRead.clear();
	mbedtls_ssl_init(&this->m_sslContext);
	mbedtls_ssl_config_init(&this->m_sslConf);
	mbedtls_x509_crt_init(&m_sslCACert);
	mbedtls_ctr_drbg_init(&m_sslCtr_drbg);
	mbedtls_entropy_init(&this->m_sslEntropy);
	mbedtls_ctr_drbg_seed(&this->m_sslCtr_drbg, mbedtls_entropy_func, &this->m_sslEntropy, NULL, 0U);
	mbedtls_debug_set_threshold(EasyCrossPlatform::Network::Socket::MBEDTLS_DEBUGLEVEL);
	this->m_Shaked = false;
	this->m_ClientSocket.Init();
	this->m_ClientSocket.ConnectCallBack = TLSAsyncClientSocket::TCPConnectCallBack;
	this->m_ClientSocket.DisconnectCallBack = TLSAsyncClientSocket::TCPDisconnectCallBack;
	this->m_ClientSocket.MsgCallBack = TLSAsyncClientSocket::TCPMsgCallBack;
	this->m_ClientSocket.ErrorCallBack = TLSAsyncClientSocket::TCPErrorCallBack;
	this->m_ClientSocket.CustomData = this;
	this->m_TMPALPNProtoList = NULL;
	this->m_TMPALPNProtoNum = 0U;
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::Connect()
{
	this->m_ClientSocket.Connect();
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::Disconnect()
{
	if (this->m_ClientSocket.isConnected() && this->m_Shaked) {
		int ret = 0;
		while (ret = mbedtls_ssl_close_notify(&this->m_sslContext) < 0) {
			if (ret != MBEDTLS_ERR_SSL_WANT_WRITE && ret != MBEDTLS_ERR_SSL_WANT_READ) {
				break;
			}
		}
		this->m_Shaked = false;
	}
	this->m_MsgWaitingForRead.clear();
	this->m_ClientSocket.Disconnect();
}

void EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::SendMsg(const std::string & Data)
{
	if (!this->m_Inited || !this->m_Shaked) {
		return;
	}
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
	this->SupportedALPNProtocols.clear();
	if (this->m_TMPALPNProtoNum > 0U) {
		for (unsigned int i = 0U; i < this->m_TMPALPNProtoNum; i++) {
			delete[] this->m_TMPALPNProtoList[i];
		}
		delete[] this->m_TMPALPNProtoList;
	}
}

EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket::~TLSAsyncClientSocket()
{
	this->Destroy();
}


void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TCPConnectCallBack(bool Succeed, void * ClientSocketPtr)
{
	int ret = 0;
	TLSSNIAsyncServerSingleConnection* myTLSClient = (TLSSNIAsyncServerSingleConnection*) ((TCPAsyncClientSocket*)ClientSocketPtr)->CustomData;
	if (Succeed) {
		mbedtls_ssl_config_defaults(&myTLSClient->m_sslConf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
		//mbedtls_ssl_conf_ca_chain(&myTLSClient->m_sslConf, &myTLSClient->m_sslCACert, NULL);
		mbedtls_ssl_conf_authmode(&myTLSClient->m_sslConf,MBEDTLS_SSL_VERIFY_NONE);
		//mbedtls_ssl_conf_handshake_timeout(&myTLSClient->m_sslConf, myTLSClient->MinHandshakeTime, myTLSClient->MaxHandshakeTime);
		if (!myTLSClient->SupportedALPNProtocols.empty()) {
			myTLSClient->m_TMPALPNProtoNum = myTLSClient->SupportedALPNProtocols.size();
			myTLSClient->m_TMPALPNProtoList = new char*[myTLSClient->m_TMPALPNProtoNum + 1];
			myTLSClient->m_TMPALPNProtoList[myTLSClient->m_TMPALPNProtoNum] = NULL;
			for (unsigned int i = 0; i < myTLSClient->m_TMPALPNProtoNum; i++) {
				myTLSClient->m_TMPALPNProtoList[i] = new char[myTLSClient->SupportedALPNProtocols[i].size() + 1];
				for (unsigned int StringIt = 0U; StringIt < myTLSClient->SupportedALPNProtocols[i].size(); StringIt++) {
					myTLSClient->m_TMPALPNProtoList[i][StringIt] = myTLSClient->SupportedALPNProtocols[i][StringIt];
				}
				myTLSClient->m_TMPALPNProtoList[i][myTLSClient->SupportedALPNProtocols[i].size()] = '\0';
			}
			mbedtls_ssl_conf_alpn_protocols(&myTLSClient->m_sslConf,(const char**) myTLSClient->m_TMPALPNProtoList);
		}
		mbedtls_ssl_conf_sni(&myTLSClient->m_sslConf, TLSSNIAsyncServerSingleConnection::TLSSNICallBack, myTLSClient);
		mbedtls_ssl_conf_rng(&myTLSClient->m_sslConf, mbedtls_ctr_drbg_random, &myTLSClient->m_sslCtr_drbg);
		mbedtls_ssl_conf_dbg(&myTLSClient->m_sslConf, TLSSNIAsyncServerSingleConnection::TLSDebugCallBack, myTLSClient);
		mbedtls_ssl_conf_min_version(&myTLSClient->m_sslConf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_2);
		mbedtls_ssl_conf_max_version(&myTLSClient->m_sslConf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3);
		const int* mCipherSuites = mbedtls_ssl_list_ciphersuites();
		mbedtls_ssl_conf_ciphersuites(&myTLSClient->m_sslConf, mCipherSuites);
		if (myTLSClient->m_SrvCerts->find("default") != myTLSClient->m_SrvCerts->end()) {
			ret = mbedtls_x509_crt_parse(&myTLSClient->m_sslDefaultCert, (const unsigned char*)myTLSClient->m_SrvCerts->at("default").first.c_str(), myTLSClient->m_SrvCerts->at("default").first.length() + 1U);
			if (ret != 0) {
				myTLSClient->onMbedTLSError(ret);
				return;
			}
			ret = mbedtls_pk_parse_key(&myTLSClient->m_sslDefaultPK, (const unsigned char*)myTLSClient->m_SrvCerts->at("default").second.first.c_str(), myTLSClient->m_SrvCerts->at("default").second.first.length() + 1U, myTLSClient->m_SrvCerts->at("default").second.second.empty() ? NULL : (const unsigned char*)myTLSClient->m_SrvCerts->at("default").second.second.c_str(), myTLSClient->m_SrvCerts->at("default").second.first.empty() ? 0U : myTLSClient->m_SrvCerts->at("default").second.second.length() + 1U);
			if (ret != 0) {
				myTLSClient->onMbedTLSError(ret);
				return;
			}
			if (myTLSClient->m_sslDefaultCert.next != NULL) {
				mbedtls_ssl_conf_ca_chain(&myTLSClient->m_sslConf, myTLSClient->m_sslDefaultCert.next, NULL);
			}
			mbedtls_ssl_conf_own_cert(&myTLSClient->m_sslConf, &myTLSClient->m_sslDefaultCert, &myTLSClient->m_sslDefaultPK);
		}
		ret = mbedtls_ssl_setup(&myTLSClient->m_sslContext, &myTLSClient->m_sslConf);
		if (ret != 0) {
			myTLSClient->onMbedTLSError(ret);
			myTLSClient->onConnected(false);
			return;
		}
		myTLSClient->serverHostName = "default";
		mbedtls_ssl_set_bio(&myTLSClient->m_sslContext, myTLSClient, TLSSNIAsyncServerSingleConnection::TLSSendCallback, TLSSNIAsyncServerSingleConnection::TLSRecvCallBack, NULL);
		myTLSClient->myWorkCls.setWork(TLSSNIAsyncServerSingleConnection::CompleteShakeProgress);
		myTLSClient->myWorkCls.StartJob(NULL, myTLSClient);
	}
	else {
		myTLSClient->onConnected(false);
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TCPDisconnectCallBack(void * ClientSocketPtr)
{
	TLSSNIAsyncServerSingleConnection* myTLSClient = (TLSSNIAsyncServerSingleConnection*)((TCPAsyncClientSocket*)ClientSocketPtr)->CustomData;
	myTLSClient->onDisconnected();
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TCPMsgCallBack(const std::vector<byte> & Data, void * ClientSocketPtr)
{
	TLSSNIAsyncServerSingleConnection* myTLSClient = (TLSSNIAsyncServerSingleConnection*)((TCPAsyncClientSocket*)ClientSocketPtr)->CustomData;
	myTLSClient->ReadTCPMsgMutex.lock();
	for (size_t i = 0U; i < Data.size(); i++) {
		myTLSClient->m_MsgWaitingForRead.push_back(Data[i]);
	}
	myTLSClient->ReadTCPMsgMutex.unlock();
	while (myTLSClient->CheckNewMsg() == true) {
		//Checking New Msg
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TCPErrorCallBack(int ErrNo, const std::string & ErrDescription, void * ClientSocketPtr)
{
	TLSSNIAsyncServerSingleConnection* myTLSClient = (TLSSNIAsyncServerSingleConnection*)((TCPAsyncClientSocket*)ClientSocketPtr)->CustomData;
	myTLSClient->onErrorOccured(ErrNo, ErrDescription);
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TLSDebugCallBack(void * TLSClientPtr, int level, const char * file, int line, const char * str)
{
	TLSSNIAsyncServerSingleConnection* myTLSClient = (TLSSNIAsyncServerSingleConnection*)TLSClientPtr;
	std::cout << file << "[" << std::to_string(line) << "]:" << str << std::endl;
	//Debug Function, nothing to do.
}

int EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TLSRecvCallBack(void * TLSClientPtr, unsigned char * buf, size_t len)
{
	TLSSNIAsyncServerSingleConnection* myTLSClient = (TLSSNIAsyncServerSingleConnection*)TLSClientPtr;
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

int EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TLSSendCallback(void * TLSClientPtr, const unsigned char * buf, size_t len)
{
	TLSSNIAsyncServerSingleConnection* myTLSClient = (TLSSNIAsyncServerSingleConnection*)TLSClientPtr;
	std::vector<byte> myMsgToSend;
	for (size_t i = 0U; i < len; i++) {
		myMsgToSend.push_back(buf[i]);
	}
	myTLSClient->SendTCPMsgMutex.lock();
	myTLSClient->m_ClientSocket->SendMsg(myMsgToSend);
	myTLSClient->SendTCPMsgMutex.unlock();
	return static_cast<int>(len);
}

int EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TLSSNICallBack(void * MyClsPtr, mbedtls_ssl_context * ssl, const unsigned char * HostName, size_t HostNameLength)
{
	TLSSNIAsyncServerSingleConnection* myTLSClient = (TLSSNIAsyncServerSingleConnection*)MyClsPtr;
	std::string mReqHostName = std::string((const char*) HostName, static_cast<std::string::size_type>(HostNameLength));
	myTLSClient->serverHostName = mReqHostName;
	int ret = 0;
	for (auto i = myTLSClient->m_SrvCerts->begin(); i != myTLSClient->m_SrvCerts->end(); i++) {
		if ((*i).first == mReqHostName) {
			//We found it!
			ret = mbedtls_x509_crt_parse(&myTLSClient->m_sslCert, (const unsigned char*)(*i).second.first.c_str(), (*i).second.first.length() + 1U);
			if (ret != 0) {
				myTLSClient->onMbedTLSError(ret);
				return -1;
			}
			ret = mbedtls_pk_parse_key(&myTLSClient->m_sslPrivateKey, (const unsigned char*)(*i).second.second.first.c_str(), (*i).second.second.first.length() + 1U, (*i).second.second.second.empty() ? NULL : (const unsigned char*)(*i).second.second.second.c_str(), (*i).second.second.second.empty() ? 0U : (*i).second.second.second.length() + 1U);
			if (ret != 0) {
				myTLSClient->onMbedTLSError(ret);
				return -1;
			}
			if (myTLSClient->m_sslCert.next != NULL) {
				mbedtls_ssl_set_hs_ca_chain(&myTLSClient->m_sslContext, myTLSClient->m_sslCert.next, NULL);
			}
			return mbedtls_ssl_set_hs_own_cert(&myTLSClient->m_sslContext, &myTLSClient->m_sslCert, &myTLSClient->m_sslPrivateKey);
		}
		else if ((*i).first[0] == '*' && mReqHostName.find((*i).first.substr(1U, (*i).first.length() - 1U), 0U) != std::string::npos) {
			//It matches!
			ret = mbedtls_x509_crt_parse(&myTLSClient->m_sslCert, (const unsigned char*)(*i).second.first.c_str(), (*i).second.first.length() + 1U);
			if (ret != 0) {
				myTLSClient->onMbedTLSError(ret);
				return -1;
			}
			ret = mbedtls_pk_parse_key(&myTLSClient->m_sslPrivateKey, (const unsigned char*)(*i).second.second.first.c_str(), (*i).second.second.first.length() + 1U, (*i).second.second.second.empty() ? NULL : (const unsigned char*) (*i).second.second.second.c_str(), (*i).second.second.second.empty() ? 0U : (*i).second.second.second.length() + 1U);
			if (ret != 0) {
				myTLSClient->onMbedTLSError(ret);
				return -1;
			}
			if (myTLSClient->m_sslCert.next != NULL) {
				mbedtls_ssl_set_hs_ca_chain(&myTLSClient->m_sslContext, myTLSClient->m_sslCert.next, NULL);
			}
			return mbedtls_ssl_set_hs_own_cert(&myTLSClient->m_sslContext, &myTLSClient->m_sslCert, &myTLSClient->m_sslPrivateKey);
		}
	}
	//Didnt find it, try to use the default cert.
	if (myTLSClient->m_SrvCerts->find("default") != myTLSClient->m_SrvCerts->end()) {
		ret = mbedtls_x509_crt_parse(&myTLSClient->m_sslCert, (const unsigned char*)myTLSClient->m_SrvCerts->at("default").first.c_str(), myTLSClient->m_SrvCerts->at("default").first.length() + 1U);
		if (ret != 0) {
			myTLSClient->onMbedTLSError(ret);
			return -1;
		}
		ret = mbedtls_pk_parse_key(&myTLSClient->m_sslPrivateKey, (const unsigned char*)myTLSClient->m_SrvCerts->at("default").second.first.c_str(), myTLSClient->m_SrvCerts->at("default").second.first.length() + 1U, myTLSClient->m_SrvCerts->at("default").second.second.empty() ? NULL : (const unsigned char*) myTLSClient->m_SrvCerts->at("default").second.second.c_str(), myTLSClient->m_SrvCerts->at("default").second.first.empty() ? 0U : myTLSClient->m_SrvCerts->at("default").second.second.length() + 1U);
		if (ret != 0) {
			myTLSClient->onMbedTLSError(ret);
			return -1;
		}
		if (myTLSClient->m_sslCert.next != NULL) {
			mbedtls_ssl_set_hs_ca_chain(&myTLSClient->m_sslContext, myTLSClient->m_sslCert.next, NULL);
		}
		return mbedtls_ssl_set_hs_own_cert(&myTLSClient->m_sslContext, &myTLSClient->m_sslCert, &myTLSClient->m_sslPrivateKey);
	}
	else {
		//No default cert set.
		return -1;
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::CompleteShakeProgress(std::thread::id myThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	TLSSNIAsyncServerSingleConnection* myTLSClient = (TLSSNIAsyncServerSingleConnection*)Parameters;
	if (!myTLSClient->m_Shaked) {
		int HandshakeRst = -1;
		while (HandshakeRst != 0 && (*RunningSign)) {
			HandshakeRst = mbedtls_ssl_handshake(&myTLSClient->m_sslContext);

			if (HandshakeRst == 0) {
				const char* NegotiatedALPNProtol = mbedtls_ssl_get_alpn_protocol(&myTLSClient->m_sslContext);
				if (NegotiatedALPNProtol != NULL) {
					myTLSClient->NegotiatedALPNProtocols.assign(NegotiatedALPNProtol);
				}
				myTLSClient->onConnected(true);
				myTLSClient->m_Shaked = true;
				if (!myTLSClient->m_MsgWaitingForRead.empty()) {
					while (myTLSClient->CheckNewMsg() == true) {
						//Checking New Msg!
					}
				}
				return;
			}
			else if (HandshakeRst != MBEDTLS_ERR_SSL_WANT_READ && HandshakeRst != MBEDTLS_ERR_SSL_WANT_WRITE) {
				myTLSClient->onConnected(false);
				myTLSClient->m_Shaked = false;
				myTLSClient->onMbedTLSError(HandshakeRst);
				myTLSClient->m_ClientSocket->Disconnect();
				return;
			}
		}
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::onDisconnected()
{
	this->myWorkCls.StopJob();
	if (!this->m_MsgWaitingForRead.empty()) {
		while (this->CheckNewMsg() == true) {
			//Checking New Msg!
		}
	}
	this->m_Shaked = false;
	mbedtls_ssl_session_reset(&this->m_sslContext);
	if (this->DisconnectCallBack != NULL) {
		this->DisconnectCallBack(this);
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::onConnected(bool Succeeded)
{
	if (this->ConnectCallBack != NULL) {
		this->ConnectCallBack(Succeeded, this);
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::onMsgCB(const std::vector<byte>& Data)
{
	if (this->MsgCallBack != NULL) {
		this->MsgCallBack(Data, this);
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::onErrorOccured(int errNo, const std::string & ErrDescription)
{
	if (this->ErrorCallBack != NULL) {
		this->ErrorCallBack(errNo, ErrDescription, this);
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::onMbedTLSError(int mbedErrNo)
{
	char ErrorMsg[200];
	mbedtls_strerror(mbedErrNo, ErrorMsg, 200);
	this->onErrorOccured(mbedErrNo, std::string(ErrorMsg));
}


void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::setWorker(SocketWorker * newWorker)
{
	this->m_ClientSocket->setWorker(newWorker);
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::setSrvCert(const std::map<std::string, std::pair<std::string, std::pair<std::string, std::string>>> *newCerts)
{
	this->m_SrvCerts = newCerts;
}

bool EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::CheckNewMsg()
{
	if (this->m_Shaked) {
		if (this->m_MsgWaitingForRead.empty()) {
			return false;
		}
		//Because New Data has been added. Call for read
		byte myBuf[25565] = "";
		int ReadRst = mbedtls_ssl_read(&this->m_sslContext, (unsigned char*)myBuf, 25565U);
		if (ReadRst == MBEDTLS_ERR_SSL_WANT_READ || ReadRst == MBEDTLS_ERR_SSL_WANT_WRITE) {
			return false;
		}
		else if (ReadRst == MBEDTLS_ERR_SSL_CONN_EOF || ReadRst == 0 || ReadRst == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
			this->Disconnect();
			return false;
		}
		else if (ReadRst < 0) {
			this->onMbedTLSError(ReadRst);
			this->Disconnect();
			return false;
		}
		std::vector<byte> realBuf;
		for (int i = 0; i < ReadRst; i++) {
			realBuf.push_back(myBuf[i]);
		}
		this->onMsgCB(realBuf);
		return true;
	}
	return false;
}

EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::getMyIpAddr()
{
	return this->m_ClientSocket->getMyIpAddr();
}

EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::getRemoteAddr()
{
	return this->m_ClientSocket->getRemoteAddr();
}

EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TLSSNIAsyncServerSingleConnection()
{

}

EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::TLSSNIAsyncServerSingleConnection(TLSSNIAsyncServerSingleConnection & oldClient)
{
	throw std::runtime_error("You cannot have a copy of this class");
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::Init()
{
	if ((this->m_Inited) || (this->m_ClientSocket == NULL)) {
		return;
	}
	int ret = 0;
	this->m_Inited = true;
	this->m_MsgWaitingForRead.clear();
	mbedtls_ssl_init(&this->m_sslContext);
	mbedtls_ssl_config_init(&this->m_sslConf);
	mbedtls_x509_crt_init(&this->m_sslCert);
	mbedtls_x509_crt_init(&this->m_sslCACert);
	mbedtls_pk_init(&this->m_sslPrivateKey);
	mbedtls_pk_init(&this->m_sslDefaultPK);
	mbedtls_ctr_drbg_init(&m_sslCtr_drbg);
	mbedtls_entropy_init(&this->m_sslEntropy);
	ret = mbedtls_ctr_drbg_seed(&this->m_sslCtr_drbg, mbedtls_entropy_func, &this->m_sslEntropy, NULL, 0U);
	if (ret != 0) {
		this->onMbedTLSError(ret);
	}
	mbedtls_debug_set_threshold(EasyCrossPlatform::Network::Socket::MBEDTLS_DEBUGLEVEL);
	this->m_Shaked = false;
	this->m_ClientSocket->Init();
	this->m_ClientSocket->ConnectCallBack = TLSSNIAsyncServerSingleConnection::TCPConnectCallBack;
	this->m_ClientSocket->DisconnectCallBack = TLSSNIAsyncServerSingleConnection::TCPDisconnectCallBack;
	this->m_ClientSocket->MsgCallBack = TLSSNIAsyncServerSingleConnection::TCPMsgCallBack;
	this->m_ClientSocket->ErrorCallBack = TLSSNIAsyncServerSingleConnection::TCPErrorCallBack;
	this->m_ClientSocket->CustomData = this;
	this->m_TMPALPNProtoList = NULL;
	this->m_TMPALPNProtoNum = 0U;
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::Disconnect()
{
	if (this->m_ClientSocket->isConnected() && this->m_Shaked) {
		int ret = 0;
		while (ret = mbedtls_ssl_close_notify(&this->m_sslContext) < 0) {
			if (ret != MBEDTLS_ERR_SSL_WANT_WRITE && ret != MBEDTLS_ERR_SSL_WANT_READ) {
				break;
			}
		}
		this->m_Shaked = false;
	}
	this->m_MsgWaitingForRead.clear();
	if (this->m_ClientSocket->isConnected()) {
		this->m_ClientSocket->Disconnect();
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::SendMsg(const std::string & Data)
{
	if (!this->m_Inited || !this->m_Shaked) {
		return;
	}
	size_t AmountWritten = 0U;
	int tmpInt = 0;
	std::string tmpData = "";
	while (AmountWritten != static_cast<int>(Data.length())) {
		tmpData = Data.substr(AmountWritten, Data.length() - AmountWritten);
		tmpInt = mbedtls_ssl_write(&this->m_sslContext, (const unsigned char*)tmpData.c_str(), Data.length() - AmountWritten);
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

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::SendMsg(const std::vector<byte>& Data)
{
	this->SendMsg(EasyCrossPlatform::Parser::StringUtil::fromBytes(Data));
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::Destroy()
{
	if (!this->m_Inited) {
		return;
	}
	this->m_Inited = false;
	this->Disconnect();
	this->m_ClientSocket->Destroy();
	this->myWorkCls.StopJob();
	mbedtls_ssl_free(&this->m_sslContext);
	mbedtls_ssl_config_free(&this->m_sslConf);
	mbedtls_ctr_drbg_free(&this->m_sslCtr_drbg);
	mbedtls_entropy_free(&this->m_sslEntropy);
	mbedtls_x509_crt_free(&this->m_sslCert);
	mbedtls_x509_crt_free(&this->m_sslCACert);
	mbedtls_pk_free(&this->m_sslPrivateKey);
	mbedtls_pk_free(&this->m_sslDefaultPK);
	if (this->m_ClientSocket != NULL) {
		delete this->m_ClientSocket;
		this->m_ClientSocket = NULL;
	}
	if (this->m_TMPALPNProtoNum > 0U) {
		for (unsigned int i = 0U; i < this->m_TMPALPNProtoNum; i++) {
			delete[] this->m_TMPALPNProtoList[i];
		}
		delete[] this->m_TMPALPNProtoList;
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::setCACert(const std::string & newCAChain)
{
	int ParseRst = mbedtls_x509_crt_parse(&this->m_sslCACert, (const unsigned char*)newCAChain.c_str(), newCAChain.length() + 1U);
	if (ParseRst<0) {
		this->onMbedTLSError(ParseRst);
	}
}

EasyCrossPlatform::Network::Socket::TLSSNIAsyncServerSingleConnection::~TLSSNIAsyncServerSingleConnection()
{
	this->Destroy();
}


void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::TCPServerNewConnCallBack(void * newClientSocket, void * ServerClassPtr)
{
	TCPAsyncClientSocket* myClient = (TCPAsyncClientSocket*)newClientSocket;
	TLSSNIAsyncServer* myTLSServer = (TLSSNIAsyncServer*) ((TCPAsyncServerSocket*)ServerClassPtr)->CustomData;
	TLSSNIAsyncServerSingleConnection* mSingleNewConn = new TLSSNIAsyncServerSingleConnection();
	mSingleNewConn->ConnectCallBack = myTLSServer->ConnectCallBack;
	mSingleNewConn->DisconnectCallBack = myTLSServer->DisconnectCallBack;
	mSingleNewConn->ErrorCallBack = myTLSServer->ErrorCallBack;
	mSingleNewConn->MsgCallBack = myTLSServer->MsgCallBack;
	mSingleNewConn->MinHandshakeTime = myTLSServer->MinHandshakeTime;
	mSingleNewConn->MaxHandshakeTime = myTLSServer->MaxHandshakeTime;
	mSingleNewConn->m_ClientSocket = myClient;
	mSingleNewConn->setSrvCert(&myTLSServer->m_SrvCerts);
	mSingleNewConn->setCACert(myTLSServer->m_sslCACert);
	mSingleNewConn->SupportedALPNProtocols = myTLSServer->SupportedALPNProtocols;
	mSingleNewConn->Init();
	myTLSServer->onConnection(mSingleNewConn);
	return;
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::TCPServerErrorCallBack(int errCode, const std::string & errInfo, void * ServerClassPtr)
{
	TLSSNIAsyncServer* myTLSServer = (TLSSNIAsyncServer*)((TCPAsyncServerSocket*)ServerClassPtr)->CustomData;
	myTLSServer->onError(errCode, errInfo);
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::onError(int ErrNo, const std::string & ErrorDescription)
{
	if (this->SrvErrorCallBack != NULL) {
		this->SrvErrorCallBack(ErrNo, ErrorDescription, this);
	}
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::onMbedTLSError(int ErrNo)
{
	char ErrorMsg[200];
	mbedtls_strerror(ErrNo, ErrorMsg, 200);
	this->onError(ErrNo, std::string(ErrorMsg));
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::onConnection(TLSSNIAsyncServerSingleConnection * NewClientPtr)
{
	if (this->ConnectionCallback == NULL) {
		delete NewClientPtr;
		return;
	}
	this->ConnectionCallback(NewClientPtr, this);
}

EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::TLSSNIAsyncServer()
{
	//Do nothing.
}

EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::TLSSNIAsyncServer(TLSSNIAsyncServer & oldServer)
{
	throw std::runtime_error("You cannot have a copy of this class");
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::setIP(const IpAddr & myIP, int QueueLength)
{
	this->m_ServerSocket.setIP(myIP, QueueLength);
}

EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::getIP()
{
	return this->m_ServerSocket.getIP();
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::Listen()
{
	this->m_ServerSocket.Listen();
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::StopListen()
{
	this->m_ServerSocket.StopListen();
}

bool EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::isListening()
{
	return this->m_ServerSocket.isListening();
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::Init()
{
	if (this->m_Inited) {
		return;
	}
	this->m_Inited = true;
	this->m_ServerSocket.ClientConnectCallBack = TLSSNIAsyncServerSingleConnection::TCPConnectCallBack;
	this->m_ServerSocket.ClientDisconnectCallBack = TLSSNIAsyncServerSingleConnection::TCPDisconnectCallBack;
	this->m_ServerSocket.ClientErrorCallBack = TLSSNIAsyncServerSingleConnection::TCPErrorCallBack;
	this->m_ServerSocket.ClientMsgCallBack = TLSSNIAsyncServerSingleConnection::TCPMsgCallBack;
	this->m_ServerSocket.ServerErrorCallBack = TLSSNIAsyncServer::TCPServerErrorCallBack;
	this->m_ServerSocket.ServerNewConnCallBack = TLSSNIAsyncServer::TCPServerNewConnCallBack;
	mbedtls_debug_set_threshold(EasyCrossPlatform::Network::Socket::MBEDTLS_DEBUGLEVEL);
	this->m_ServerSocket.Init();
	this->m_ServerSocket.CustomData = this;
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::Destroy()
{
	if (!this->m_Inited) {
		return;
	}
	this->m_Inited = false;
	this->m_ServerSocket.Destroy();
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::setWorker(SocketWorker * newWorker)
{
	this->m_ServerSocket.setWorkers(newWorker);
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::setServerCerts(std::map<std::string, std::pair<std::string, std::pair<std::string,std::string>>>& newCerts)
{
	this->m_SrvCerts = newCerts;
}

void EasyCrossPlatform::Network::Socket::TLSSNIAsyncServer::setCACerts(const std::string& Cert)
{
	this->m_sslCACert = Cert;
}
