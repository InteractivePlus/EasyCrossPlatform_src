#include <XSYDUDPSocket.h>
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::m_uv_read_cb(uv_udp_t * handle, ssize_t nread, const uv_buf_t * buf, const sockaddr * remoteaddr, unsigned flags)
{
	UDPAsyncClientAndServerSocket* MyClass = (UDPAsyncClientAndServerSocket*) handle->data;
	std::string errMsg;
	if (nread < 0) {
		errMsg = uv_err_name(nread);
	}
	std::string data;
	Socket::IpAddr RemoteAddr;
	if (remoteaddr != NULL) {
		RemoteAddr.setIPAddress(((remoteaddr)));
	}
	if (nread > 0) {
		data.assign(buf->base, nread);
	}
	if (nread < 0){
		MyClass->onError(nread, errMsg);
	}
	else if (nread > 0) {
		MyClass->onMsg(RemoteAddr, data);
	}
	if (buf->base) {
		free(buf->base);
	}
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::m_uv_send_cb(uv_udp_send_t * req, int status)
{
	UDPAsyncClientAndServerSocket* MyClass = (UDPAsyncClientAndServerSocket*)req->handle->data;
	if (status < 0) {
		MyClass->onError(status, uv_err_name(status));
	}
	free(req);
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::m_uv_alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::UDPAsyncClientAndServerSocket(const IpAddr & ListeningAddr, SocketWorker * mWorker)
{
	this->m_myAddr = ListeningAddr;
	this->mySocketWorker = mWorker;
	this->Init();
}

EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::UDPAsyncClientAndServerSocket(UDPAsyncClientAndServerSocket & oldUDP)
{
	throw std::runtime_error("You cannot have a copy of this class.");
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::SetServerMsgCallBack(MailBoxRecieverMsgCallBack mCB)
{
	this->m_MsgCallBack = mCB;
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::SetServerErrCallBack(MailBoxRecieverErrorCallBack mCB)
{
	this->m_ErrCallBack = mCB;
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::SendMsg(const IpAddr& Destination, const std::string& Data)
{
	uv_udp_send_t* mySendReq = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
	IpAddr MyDestination = Destination;
	const sockaddr* myAddr = MyDestination.getIPAddress();

	char* tmpData = new char[Data.length() + 1];
	for (unsigned int i = 0; i < Data.length(); i++) {
		tmpData[i] = Data[i];
	}
	tmpData[Data.length()] = '\0';
	uv_buf_t myBuffer = uv_buf_init(tmpData, Data.length());
	
	int Result = uv_udp_send(mySendReq, this->m_SocketHandle, &myBuffer, 1, myAddr, UDPAsyncClientAndServerSocket::m_uv_send_cb);
	if (Result < 0) {
		throw std::runtime_error(uv_err_name(Result));
	}
	delete[] tmpData;
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::SendMsg(const IpAddr & Destination, const std::vector<byte>& Data)
{
	try {
		this->SendMsg(Destination, EasyCrossPlatform::Parser::StringUtil::fromBytes(Data));
	}
	catch (std::runtime_error e) {
		throw e;
	}
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::StartListen()
{
	int recvState = uv_udp_recv_start(this->m_SocketHandle, UDPAsyncClientAndServerSocket::m_uv_alloc_buffer, UDPAsyncClientAndServerSocket::m_uv_read_cb);
	if (recvState < 0) {
		throw std::runtime_error(uv_err_name(recvState));
		return;
	}
	this->m_isListening = true;
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::StopListen()
{
	if (this->m_isListening) {
		uv_udp_recv_stop(this->m_SocketHandle);
		this->m_isListening = false;
	}
}
bool EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::isListening()
{
	return this->m_isListening;
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::Init()
{
	this->m_SocketHandle = new uv_udp_t;
	this->mySocketWorker->Increment();
	
	uv_udp_init(this->mySocketWorker->m_uv_loop.get(), this->m_SocketHandle);
	this->m_SocketHandle->data = (void*)this;
	const sockaddr* myAddress = this->m_myAddr.getIPAddress();
	int state = uv_udp_bind(this->m_SocketHandle, myAddress, 0);
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::Destroy()
{
	uv_close((uv_handle_t*)this->m_SocketHandle, NULL);
	this->mySocketWorker->Decrement();
	if (this->m_SocketHandle != NULL) {
		delete this->m_SocketHandle;
		this->m_SocketHandle = NULL;
	}
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::onMsg(const IpAddr& SourceIP, const std::string& data)
{
	if (this->m_MsgCallBack != NULL) {
		this->m_MsgCallBack(EasyCrossPlatform::Parser::StringUtil::toBytes(data), SourceIP, this);
	}
}
void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::onError(int errCode, const std::string& errDescription)
{
	if (this->m_ErrCallBack != NULL) {
		this->m_ErrCallBack(errCode, errDescription, this);
	}
}

EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientAndServerSocket::~UDPAsyncClientAndServerSocket()
{
	this->Destroy();
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientSocket::m_uv_send_cb(uv_udp_send_t * req, int status)
{
	UDPAsyncClientAndServerSocket* MyClass = (UDPAsyncClientAndServerSocket*)req->handle->data;
	free(req);
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientSocket::Init()
{
	this->m_SocketHandle = new uv_udp_t;
	this->mySocketWorker->Increment();

	uv_udp_init(this->mySocketWorker->m_uv_loop.get(), this->m_SocketHandle);
	this->m_SocketHandle->data = (void*) this;
	if (this->m_myAddr.getPort() != 0U) {
		const sockaddr* myAddress = this->m_myAddr.getIPAddress();
		int state = uv_udp_bind(this->m_SocketHandle, myAddress, 0);
	}
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientSocket::Destroy()
{
	uv_close((uv_handle_t*)this->m_SocketHandle, NULL);
	this->mySocketWorker->Decrement();
	if (this->m_SocketHandle != NULL) {
		delete this->m_SocketHandle;
		this->m_SocketHandle = NULL;
	}
}

EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientSocket::UDPAsyncClientSocket(SocketWorker * mWorker, const IpAddr& BindAddr)
{
	this->mySocketWorker = mWorker;
	this->m_myAddr = BindAddr;
	this->Init();
}

EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientSocket::UDPAsyncClientSocket(UDPAsyncClientSocket & oldUDP)
{
	throw std::runtime_error("You cannot have a copy of this class.");
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientSocket::SendMsg(const IpAddr & Destination, const std::string & Data)
{
	uv_udp_send_t* mySendReq = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
	IpAddr MyDestination = Destination;
	const sockaddr* myAddr = MyDestination.getIPAddress();
	char* tmpData = new char[Data.length() + 1];
	for (unsigned int i = 0; i < Data.length(); i++) {
		tmpData[i] = Data[i];
	}
	tmpData[Data.length()] = '\0';
	uv_buf_t myBuffer = uv_buf_init(tmpData, Data.length());

	int Result = uv_udp_send(mySendReq, this->m_SocketHandle, &myBuffer, 1, myAddr, UDPAsyncClientSocket::m_uv_send_cb);
	if (Result < 0) {
		throw std::runtime_error(uv_err_name(Result));
	}
	delete[] tmpData;
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientSocket::SendMsg(const IpAddr & Destination, const std::vector<byte>& Data)
{
	try {
		SendMsg(Destination, EasyCrossPlatform::Parser::StringUtil::fromBytes(Data));
	}
	catch (std::runtime_error e) {
		throw e;
	}
}

EasyCrossPlatform::Network::Socket::UDP::UDPAsyncClientSocket::~UDPAsyncClientSocket()
{
	this->Destroy();
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::Init()
{
	this->m_SocketHandle = new uv_udp_t;
	this->mySocketWorker->Increment();

	uv_udp_init(this->mySocketWorker->m_uv_loop.get(), this->m_SocketHandle);
	this->m_SocketHandle->data = (void*)this;
	const sockaddr* myAddress = this->m_myAddr.getIPAddress();
	int state = uv_udp_bind(this->m_SocketHandle, myAddress, 0);
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::Destroy()
{
	uv_close((uv_handle_t*)this->m_SocketHandle, NULL);
	this->mySocketWorker->Decrement();
	if (this->m_SocketHandle != NULL) {
		delete this->m_SocketHandle;
		this->m_SocketHandle = NULL;
	}
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::onError(int ErrNo, const std::string & ErrDescription)
{
	if (this->m_ErrCallBack != NULL) {
		this->m_ErrCallBack(ErrNo, ErrDescription, this);
	}
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::onMsg(const IpAddr & mIP, const std::string & Data)
{
	if (this->m_MsgCallBack != NULL) {
		this->m_MsgCallBack(EasyCrossPlatform::Parser::StringUtil::toBytes(Data), mIP, this);
	}
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::m_uv_read_cb(uv_udp_t * handle, ssize_t nread, const uv_buf_t * buf, const sockaddr * remoteaddr, unsigned flags)
{
	UDPAsyncServerSocket* MyClass = (UDPAsyncServerSocket*)handle->data;
	std::string errMsg;
	if (nread < 0) {
		errMsg = uv_err_name(nread);
	}
	std::string data;
	Socket::IpAddr RemoteAddr;
	if (remoteaddr != NULL) {
		RemoteAddr.setIPAddress(remoteaddr);
	}
	if (nread > 0) {
		data.assign(buf->base, nread);
	}
	if (nread < 0) {
		MyClass->onError(nread, errMsg);
	}
	else if (nread > 0) {
		MyClass->onMsg(RemoteAddr, data);
	}
	if (buf->base) {
		free(buf->base);
	}
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::m_uv_alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::UDPAsyncServerSocket(const IpAddr & ListeningAddr, SocketWorker * mWorker)
{
	this->m_myAddr = ListeningAddr;
	this->mySocketWorker = mWorker;
}

EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::UDPAsyncServerSocket(UDPAsyncServerSocket & oldServer)
{
	throw std::runtime_error("You cannot have a copy of this class.");
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::SetServerMsgCallBack(MailBoxRecieverMsgCallBack mCB)
{
	this->m_MsgCallBack = mCB;
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::SetServerErrCallBack(MailBoxRecieverErrorCallBack mCB)
{
	this->m_ErrCallBack = mCB;
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::StartListen()
{
	int recvState = uv_udp_recv_start(this->m_SocketHandle, UDPAsyncServerSocket::m_uv_alloc_buffer, UDPAsyncServerSocket::m_uv_read_cb);
	if (recvState < 0) {
		throw std::runtime_error(uv_err_name(recvState));
		return;
	}
	this->m_isListening = true;
}

void EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::StopListen()
{
	if (this->m_isListening) {
		uv_udp_recv_stop(this->m_SocketHandle);
		this->m_isListening = false;
	}
}

bool EasyCrossPlatform::Network::Socket::UDP::UDPAsyncServerSocket::isListening()
{
	return this->m_isListening;
}
