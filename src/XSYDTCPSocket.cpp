#include <XSYDTCPSocket.h>

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_connect_cb(uv_connect_t * req, int status)
{
	uv_tcp_t* mytcpHandle = (uv_tcp_t*)req->handle;
	free(req);
	TCPAsyncClientSocket* myClass = (TCPAsyncClientSocket*) req->data;
	if (status < 0) {
		myClass->onConnected(false);
	}
	else {
		myClass->m_Connected = true;
		uv_read_start((uv_stream_t*)mytcpHandle, TCPAsyncClientSocket::m_uv_alloc_buffer, TCPAsyncClientSocket::m_uv_read_cb);
		myClass->onConnected(true);
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_close_cb(uv_handle_t * handle)
{
	TCPAsyncClientSocket* MyClass = (TCPAsyncClientSocket*) handle->data;
	MyClass->Closing = false;
	if (MyClass->m_Connected) {
		MyClass->m_Connected = false;
		MyClass->onDisconnect();
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_read_cb(uv_stream_t * stream, ssize_t nread, const uv_buf_t * buf)
{
	std::vector<byte> data;
	std::string dataStr;
	TCPAsyncClientSocket* myClass = (TCPAsyncClientSocket*)stream->data;
	std::string myErrMsg;
	
	if (nread < 0 && nread != UV_EOF) {
		myErrMsg = uv_err_name(nread);
	}
	if (nread > 0) {
		dataStr.assign(buf->base, static_cast<std::string::size_type>(nread));
		data = EasyCrossPlatform::Parser::StringUtil::toBytes(dataStr);
	}
	if (nread > 0) {
		if (myClass->m_Connected) {
			myClass->onMsg(data);
		}
	}
	if (nread == UV_EOF) {
		myClass->Disconnect();
	}
	else if (nread < 0) {
		myClass->onError(nread,myErrMsg);
		myClass->Disconnect();
	}
	else { //nread == 0
		//Everything OK, nothing read, do nothing
	}
	if (buf->base != NULL) {
		free(buf->base);
	}
	return;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_shutdown_cb(uv_shutdown_t * req, int state)
{
	if (state < 0) {
		TCPAsyncClientSocket* myClass = (TCPAsyncClientSocket*)req->data;
		myClass->onError(state, uv_err_name(state));
		myClass->Closing = false;
	}
	free(req);
	return;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_write_cb(uv_write_t * req, int status)
{
	delete[] req->data;
	free(req);
	return;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	buf->base = (char*)malloc(suggested_size * sizeof(char));
	buf->len = suggested_size;
}

EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::TCPAsyncClientSocket() {
	
}

EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::TCPAsyncClientSocket(const IpAddr& RemoteIP){
	this->m_remoteAddr = RemoteIP;
	
}
EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::TCPAsyncClientSocket(const TCPAsyncClientSocket & oldClient)
{
	throw std::runtime_error("You cannot have a copy of this class");
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::setWorker(SocketWorker* socketWorker)
{
	this->mySocketWorker = socketWorker;
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::Init()
{
	if (this->Inited) {
		return;
	}
	else if (this->mySocketWorker == NULL) {
		return;
	}

	this->Inited = true;
	this->m_ClientSocketHandle = std::shared_ptr<uv_tcp_t>(new uv_tcp_t);
	this->m_ClientSocketHandle.get()->data = (void*)this;
	
	if (this->mySocketWorker->m_num_Client == 0U) {
		this->mySocketWorker->Start();
	}
	this->mySocketWorker->m_num_Client++;

	uv_tcp_init(this->mySocketWorker->m_uv_loop.get(), this->m_ClientSocketHandle.get());
	
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::Connect()
{
	if (!this->Inited) {
		this->onError(-1, "You cannot connect without intializing");
		return;
	}
	uv_connect_t *myReadReq = (uv_connect_t*) malloc(sizeof(uv_connect_t));
	myReadReq->data = (void*) this;
	sockaddr myAddress = this->m_remoteAddr.getIPAddress();
	uv_tcp_connect(myReadReq, this->m_ClientSocketHandle.get(), &myAddress, EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_connect_cb);
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::setRemoteIPAddr(const IpAddr & newIP)
{
	this->m_remoteAddr = newIP;
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::setSelfPort(const unsigned short Port)
{
	IpAddr mIp;
	if (this->m_remoteAddr.addrIsIPV4()) {
		 mIp.setIPAddress("0.0.0.0", Port, true);
	}
	else {
		mIp.setIPAddress("::", Port, false);
	}
	sockaddr mAddr = mIp.getIPAddress();
	int rst = uv_tcp_bind(this->m_ClientSocketHandle.get(), &mAddr, 0);
	if (rst < 0) {
		throw std::runtime_error("The Port cannot be binded");
	}
}
EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::getMyIpAddr()
{
	if (this->m_Connected) {
		sockaddr mySocketAddr;
		IpAddr myIP;
		int ipStructSize = sizeof(sockaddr);
		uv_tcp_getsockname(this->m_ClientSocketHandle.get(), &mySocketAddr, &ipStructSize);
		myIP.setIPAddress(mySocketAddr);
		return myIP;
	}
	return IpAddr("127.0.0.1",0U,true);
}
EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::getRemoteAddr()
{
	if (this->m_Connected) {
		sockaddr mySocketAddr;
		IpAddr remoteIP;
		int ipStructSize = sizeof(sockaddr);
		uv_tcp_getpeername(this->m_ClientSocketHandle.get(), &mySocketAddr, &ipStructSize);
		remoteIP.setIPAddress(mySocketAddr);
		return remoteIP;
	}
	return this->m_remoteAddr;
}
bool EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::isConnected()
{
	return this->m_Connected;
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::SendMsg(const std::string & Msg)
{
	if (!this->m_Connected || !this->Inited || this->Closing) {
		return;
	}
	uv_write_t* myWriteRequest = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_buf_t myWriteBuf;
	//myWriteRequest->data = (void*)this->mySocketWorker;
	
	char* myBufSpace = new char[Msg.length()];
	for (unsigned int i = 0; i < Msg.length(); i++) {
		myBufSpace[i] = Msg[i];
	}
	
	myWriteBuf = uv_buf_init(myBufSpace, static_cast<unsigned int>(Msg.length()));
	myWriteRequest->data = myBufSpace;
	uv_write(myWriteRequest, (uv_stream_t*)this->m_ClientSocketHandle.get(), &myWriteBuf, 1, TCPAsyncClientSocket::m_uv_write_cb);
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::SendMsg(const std::vector<byte>& Msg)
{
	this->SendMsg(EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg));
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::Disconnect()
{
	if ((!this->m_Connected) || (this->Closing)) {
		return;
	}
	if (this->Inited) {
		this->Closing = true;
		uv_read_stop((uv_stream_t*)this->m_ClientSocketHandle.get());
		uv_close((uv_handle_t*) this->m_ClientSocketHandle.get(), TCPAsyncClientSocket::m_uv_close_cb);
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::Destroy()
{
	if (!this->Inited) {
		return;
	}
	while (this->Closing) {
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
	}
	this->Inited = false;
	this->Disconnect();
	this->mySocketWorker->m_num_Client--;
	if (this->mySocketWorker->m_num_Client == 0U) {
		this->mySocketWorker->Stop();
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::onConnected(bool Succeeded)
{
	if (this->ConnectCallBack != NULL) {
		this->ConnectCallBack(Succeeded, this);
	}
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::onMsg(const std::vector<byte> & Msg)
{
	if (this->MsgCallBack != NULL) {
		this->MsgCallBack(Msg,this);
	}
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::onDisconnect()
{
	if (this->DisconnectCallBack != NULL) {
		this->DisconnectCallBack(this);
	}
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::onError(int errCode, const std::string& errDescription)
{
	if (this->ErrorCallBack != NULL) {
		this->ErrorCallBack(errCode, errDescription, this);
	}
}
EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::~TCPAsyncClientSocket()
{
	this->Destroy();
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::onConnection(TCPAsyncClientSocket* newClient)
{
	if (this->ServerNewConnCallBack != NULL) {
		this->ServerNewConnCallBack(newClient, this);
	}
	newClient->onConnected(true);
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::onError(int errCode, const std::string & errorDescription)
{
	if (this->ServerErrorCallBack != NULL) {
		this->ServerErrorCallBack(errCode, errorDescription, this);
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::m_uv_connection_cb(uv_stream_t * server, int status)
{
	TCPAsyncServerSocket* MyClass = (TCPAsyncServerSocket*) (server->data);
	TCPAsyncClientSocket* MyNewClient = NULL;
	int acceptState = 0;
	if (status < 0) {
		MyClass->onError(status, uv_err_name(status));
		return;
	}
	else if (acceptState != 0) {
			
	}
	else if (MyClass->isListening()) {
		MyNewClient = new TCPAsyncClientSocket();
		MyNewClient->setWorker(MyClass->myListenWorker);
		MyNewClient->Init();
		acceptState = uv_accept(server, (uv_stream_t*)MyNewClient->m_ClientSocketHandle.get());
		if (acceptState == 0) {
			uv_read_start((uv_stream_t*)MyNewClient->m_ClientSocketHandle.get(), TCPAsyncClientSocket::m_uv_alloc_buffer, TCPAsyncClientSocket::m_uv_read_cb);
			MyNewClient->m_Connected = true;
		}
		else {
			MyNewClient->m_Connected = false;
			MyClass->onError(acceptState, uv_err_name(acceptState));
			delete MyNewClient;
			return;
		}
		MyNewClient->ConnectCallBack = MyClass->ClientConnectCallBack;
		MyNewClient->DisconnectCallBack = MyClass->ClientDisconnectCallBack;
		MyNewClient->ErrorCallBack = MyClass->ClientErrorCallBack;
		MyNewClient->MsgCallBack = MyClass->ClientMsgCallBack;
		MyClass->onConnection(MyNewClient);
	}
}

EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::TCPAsyncServerSocket()
{
	
}

EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::TCPAsyncServerSocket(const IpAddr & myIP, int QueLength)
{
	this->m_myIP = myIP;
	this->m_QueueLength = QueLength;
}

EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::TCPAsyncServerSocket(const TCPAsyncServerSocket & oldServer)
{
	throw std::runtime_error("You cannot have a copy of this class");
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::setWorkers(SocketWorker* Worker)
{
	this->myListenWorker = Worker;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::Init()
{
	if (this->hasInted) {
		return;
	}
	else if (this->myListenWorker == NULL) {
		return;
	}
	this->hasInted = true;
	if (this->myListenWorker->m_num_Client == 0U) {
		this->myListenWorker->Start();
	}
	uv_tcp_init(this->myListenWorker->m_uv_loop.get(), &this->m_SocketHandle);
	this->m_SocketHandle.data = (void*)this;
	this->myListenWorker->m_num_Client++;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::Destroy()
{
	if (!this->hasInted) {
		return;
	}
	this->hasInted = false;

	this->myListenWorker->m_num_Client--;
	if (this->myListenWorker->m_num_Client == 0U) {
		this->myListenWorker->Stop();
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::setIP(const IpAddr & myIP, int QueLength)
{
	this->m_myIP = myIP;
	this->m_QueueLength = QueLength;
}

EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::getIP()
{
	return this->m_myIP;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::Listen()
{
	if (!this->hasInted) {
		this->onError(-1, "You cannot Listen on your socket without Intializing");
		return;
	}
	else if(this->m_QueueLength == 0){
		this->onError(-1, "You havent specified your IP Address to listen, and your queue length");
		return;
	}
	sockaddr myAddr = this->m_myIP.getIPAddress();
	
	int bindState = uv_tcp_bind(&this->m_SocketHandle, &myAddr, 0);
	if (bindState < 0) {
		this->onError(bindState, uv_err_name(bindState));
		return;
	}
	int listenState = uv_listen((uv_stream_t*) &this->m_SocketHandle, this->m_QueueLength, TCPAsyncServerSocket::m_uv_connection_cb);
	if (listenState < 0) {
		this->onError(listenState, uv_err_name(listenState));
		return;
	}
	this->m_isListening = true;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::Listen(const IpAddr & myIP, int QueLength)
{
	this->m_myIP = myIP;
	this->m_QueueLength = QueLength;
	this->Listen();
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::StopListen()
{
	this->m_isListening = false;
	uv_close((uv_handle_t*) &this->m_SocketHandle, NULL);
}

bool EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::isListening()
{
	return this->m_isListening;
}

EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::~TCPAsyncServerSocket()
{
	this->Destroy();
}
