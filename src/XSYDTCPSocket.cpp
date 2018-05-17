#include <XSYDTCPSocket.h>

EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::TCPAsyncClientSocket(SocketWorker * mWorker)
{
	this->m_SocketWorker = mWorker;
	this->Init();
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::m_uv_connect_cb(uv_connect_t * req, int status)
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

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::m_uv_close_cb(uv_handle_t * handle)
{
	TCPAsyncClientSocket* MyClass = (TCPAsyncClientSocket*) handle->data;
	MyClass->Closing = false;
	if (MyClass->m_Connected) {
		MyClass->m_Connected = false;
		MyClass->onDisconnect();
	}
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::m_uv_read_cb(uv_stream_t * stream, ssize_t nread, const uv_buf_t * buf)
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

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::m_uv_shutdown_cb(uv_shutdown_t * req, int state)
{
	if (state < 0) {
		TCPAsyncClientSocket* myClass = (TCPAsyncClientSocket*)req->data;
		myClass->onError(state, uv_err_name(state));
		myClass->Closing = false;
	}
	free(req);
	return;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::m_uv_write_cb(uv_write_t * req, int status)
{
	delete[] req->data;
	free(req);
	return;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::m_uv_alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	buf->base = (char*)malloc(suggested_size * sizeof(char));
	buf->len = suggested_size;
}

EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::TCPAsyncClientSocket(const IpAddr & RemoteIP, SocketWorker* mWorker, unsigned short SelfPort)
{
	this->m_SocketWorker = mWorker;
	this->m_remoteAddr = RemoteIP;
	this->Init();
	if (SelfPort != 0U) {
		IpAddr mIp;
		if (this->m_remoteAddr.addrIsIPV4()) {
			mIp.setIPAddress("0.0.0.0", SelfPort);
		}
		else {
			mIp.setIPAddress("::", SelfPort);
		}
		const sockaddr* tempAddr = mIp.getIPAddress();
		uv_tcp_bind(this->m_ClientSocketHandle, tempAddr, 0);
	}
}
EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::TCPAsyncClientSocket(const TCPAsyncClientSocket & oldClient)
{
	throw std::runtime_error("You cannot have a copy of this class");
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::SetConnectCallBack(StandardClientConnCallBack mCallBack)
{
	this->m_ConnectCallBack = mCallBack;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::SetMsgCallBack(StandardClientMsgCallBack mCallBack)
{
	this->m_MsgCallBack = mCallBack;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::SetDisconnectCallBack(StandardClientDisconnectCallBack mCallBack)
{
	this->m_DisconnectCallBack = mCallBack;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::SetErrorCallBack(StandardClientErrorCallBack mCallBack)
{
	this->m_ErrorCallBack = mCallBack;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::Init()
{
	this->m_ClientSocketHandle = new uv_tcp_t;
	this->m_ClientSocketHandle->data = (void*)this;
	
	this->m_SocketWorker->Increment();

	uv_tcp_init(this->m_SocketWorker->m_uv_loop.get(), this->m_ClientSocketHandle);
	
}
void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::Connect()
{
	if (this->m_Connected) {
		throw std::runtime_error("You cannot connect again when the socket state is connected!");
	}
	uv_connect_t *myReadReq = (uv_connect_t*) malloc(sizeof(uv_connect_t));
	myReadReq->data = (void*) this;
	const sockaddr* myAddress = this->m_remoteAddr.getIPAddress();
	int Result = uv_tcp_connect(myReadReq, this->m_ClientSocketHandle, myAddress, EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::m_uv_connect_cb);
	if (Result < 0) {
		throw std::runtime_error(uv_err_name(Result));
	}
}
EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::getMyIpAddr()
{
	if (this->m_Connected) {
		sockaddr mySocketAddr;
		IpAddr myIP;
		int ipStructSize = sizeof(sockaddr);
		uv_tcp_getsockname(this->m_ClientSocketHandle, &mySocketAddr, &ipStructSize);
		myIP.setIPAddress(&mySocketAddr);
		return myIP;
	}
	return IpAddr("127.0.0.1",0U);
}
EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::getRemoteAddr()
{
	if (this->m_Connected) {
		sockaddr mySocketAddr;
		IpAddr remoteIP;
		int ipStructSize = sizeof(sockaddr);
		uv_tcp_getpeername(this->m_ClientSocketHandle, &mySocketAddr, &ipStructSize);
		remoteIP.setIPAddress(&mySocketAddr);
		return remoteIP;
	}
	return this->m_remoteAddr;
}
bool EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::isConnected()
{
	return this->m_Connected;
}
void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::SendMsg(const std::string & Msg)
{
	if (!this->m_Connected || this->Closing) {
		throw std::runtime_error("You cannot send msg while the socket is closing or not connected");
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
	int Result = uv_write(myWriteRequest, (uv_stream_t*)this->m_ClientSocketHandle, &myWriteBuf, 1, TCPAsyncClientSocket::m_uv_write_cb);
	if (Result < 0) {
		throw std::runtime_error(uv_err_name(Result));
	}
}
void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::SendMsg(const std::vector<byte>& Msg)
{
	this->SendMsg(EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg));
}
void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::Disconnect()
{
	if ((!this->m_Connected) || (this->Closing)) {
		return;
	}
	this->Closing = true;
	uv_read_stop((uv_stream_t*)this->m_ClientSocketHandle);
	uv_close((uv_handle_t*) this->m_ClientSocketHandle, TCPAsyncClientSocket::m_uv_close_cb);
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::Destroy()
{
	while (this->Closing) {
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
	}
	this->Disconnect();
	this->m_SocketWorker->Decrement();
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::onConnected(bool Succeeded)
{
	if (this->m_ConnectCallBack != NULL) {
		this->m_ConnectCallBack(Succeeded, this);
	}
}
void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::onMsg(const std::vector<byte> & Msg)
{
	if (this->m_MsgCallBack != NULL) {
		this->m_MsgCallBack(Msg,this);
	}
}
void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::onDisconnect()
{
	if (this->m_DisconnectCallBack != NULL) {
		this->m_DisconnectCallBack(this);
	}
}
void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::onError(int errCode, const std::string& errDescription)
{
	if (this->m_ErrorCallBack != NULL) {
		this->m_ErrorCallBack(errCode, errDescription, this);
	}
}
EasyCrossPlatform::Network::Socket::TCP::TCPAsyncClientSocket::~TCPAsyncClientSocket()
{
	this->Destroy();
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::onConnection(TCPAsyncClientSocket* newClient)
{
	if (this->m_ServerNewConnCallBack != NULL) {
		this->m_ServerNewConnCallBack(newClient, this);
	}
	newClient->onConnected(true);
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::onError(int errCode, const std::string & errorDescription)
{
	if (this->m_ServerErrorCallBack != NULL) {
		this->m_ServerErrorCallBack(errCode, errorDescription, this);
	}
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::m_uv_connection_cb(uv_stream_t * server, int status)
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
		MyNewClient = new TCPAsyncClientSocket(MyClass->myListenWorker);
		acceptState = uv_accept(server, (uv_stream_t*)MyNewClient->m_ClientSocketHandle);
		if (acceptState == 0) {
			uv_read_start((uv_stream_t*)MyNewClient->m_ClientSocketHandle, TCPAsyncClientSocket::m_uv_alloc_buffer, TCPAsyncClientSocket::m_uv_read_cb);
			MyNewClient->m_Connected = true;
		}
		else {
			MyNewClient->m_Connected = false;
			MyClass->onError(acceptState, uv_err_name(acceptState));
			delete MyNewClient;
			return;
		}
		MyNewClient->SetConnectCallBack(MyClass->m_ClientConnectCallBack);
		MyNewClient->SetDisconnectCallBack(MyClass->m_ClientDisconnectCallBack);
		MyNewClient->SetErrorCallBack(MyClass->m_ClientErrorCallBack);
		MyNewClient->SetMsgCallBack(MyClass->m_ClientMsgCallBack);
		MyClass->onConnection(MyNewClient);
	}
}

EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::TCPAsyncServerSocket(const IpAddr& myIP, SocketWorker * mWorker, int QueLength)
{
	this->m_myIP = myIP;
	this->m_QueueLength = QueLength;
	this->myListenWorker = mWorker;
	this->Init();
}

EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::TCPAsyncServerSocket(const TCPAsyncServerSocket & oldServer)
{
	throw std::runtime_error("You cannot have a copy of this class");
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::SetClientConnectCallBack(StandardClientConnCallBack mCB)
{
	this->m_ClientConnectCallBack = mCB;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::SetClientMsgCallBack(StandardClientMsgCallBack mCB)
{
	this->m_ClientMsgCallBack = mCB;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::SetClientDisconnectCallBack(StandardClientDisconnectCallBack mCB)
{
	this->m_ClientDisconnectCallBack = mCB;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::SetClientErrorCallBack(StandardClientErrorCallBack mCB)
{
	this->m_ClientErrorCallBack = mCB;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::SetServerNewConnCallBack(StandardServerNewConnectionCallBack mCB)
{
	this->m_ServerNewConnCallBack = mCB;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::SetServerErrorCallBack(StandardServerErrorCallBack mCB)
{
	this->m_ServerErrorCallBack = mCB;
}


void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::Init()
{
	this->myListenWorker->Increment();
	uv_tcp_init(this->myListenWorker->m_uv_loop.get(), &this->m_SocketHandle);
	this->m_SocketHandle.data = (void*)this;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::Destroy()
{
	this->myListenWorker->Decrement();
}

EasyCrossPlatform::Network::Socket::IpAddr EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::getIP()
{
	return this->m_myIP;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::StartListen()
{
	const sockaddr* myAddr = this->m_myIP.getIPAddress();
	int bindState = uv_tcp_bind(&this->m_SocketHandle, myAddr, 0);
	if (bindState < 0) {
		this->onError(bindState, uv_err_name(bindState));
		return;
	}
	int listenState = uv_listen((uv_stream_t*)&this->m_SocketHandle, this->m_QueueLength, TCPAsyncServerSocket::m_uv_connection_cb);
	if (listenState < 0) {
		throw std::runtime_error(uv_err_name(listenState));
		return;
	}
	this->m_isListening = true;
}

void EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::StopListen()
{
	this->m_isListening = false;
	uv_close((uv_handle_t*) &this->m_SocketHandle, NULL);
}

bool EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::isListening()
{
	return this->m_isListening;
}

EasyCrossPlatform::Network::Socket::TCP::TCPAsyncServerSocket::~TCPAsyncServerSocket()
{
	this->Destroy();
}
