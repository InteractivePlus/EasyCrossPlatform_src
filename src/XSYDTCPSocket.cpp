#include <XSYDTCPSocket.h>

std::map<uv_tcp_t*, std::vector<EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*>> EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_MyClassPtrs;
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_connect_cb(uv_connect_t * req, int status)
{
	uv_tcp_t* mytcpHandle = (uv_tcp_t*)req->handle;
	free(req);
	std::vector<TCPAsyncClientSocket*> &myClasses = TCPAsyncClientSocket::m_MyClassPtrs[mytcpHandle];
	for (auto iter = myClasses.begin(); iter != myClasses.end(); iter++) {
		if (status < 0) {
			(*iter)->onConnected(false);
		}
		else {
			(*iter)->onConnected(true);
			(*iter)->m_Connected = true;
			uv_read_start((uv_stream_t*)mytcpHandle, TCPAsyncClientSocket::m_uv_alloc_buffer, TCPAsyncClientSocket::m_uv_read_cb);
		}
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_close_cb(uv_handle_t * handle)
{
	std::vector<TCPAsyncClientSocket*> MyClasses = TCPAsyncClientSocket::m_MyClassPtrs[((uv_tcp_t*)handle)];

	if (!MyClasses.empty()) {
		for (auto iter = MyClasses.begin(); iter != MyClasses.end(); iter++) {
			(*iter)->Closing = false;
			if ((*iter)->m_Connected) {
				(*iter)->m_Connected = false;
				(*iter)->onDisconnect();
			}
			if (MyClasses.empty()) {
				break;
			}
		}
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_read_cb(uv_stream_t * stream, ssize_t nread, const uv_buf_t * buf)
{
	std::string data;
	std::vector<TCPAsyncClientSocket*> &myClasses = TCPAsyncClientSocket::m_MyClassPtrs[(uv_tcp_t*)stream];
	std::string myErrMsg;
	
	if (nread < 0 && nread != UV_EOF) {
		myErrMsg = uv_err_name(nread);
	}
	if (nread > 0) {
		data.assign(buf->base, nread);
	}
	for (auto iter = myClasses.begin(); iter != myClasses.end(); iter++) {
		if (nread > 0) {
			if ((*iter)->m_Connected) {
				(*iter)->onMsg(data);
			}
		}
		if (nread == UV_EOF) {
			//EOF代表连接断开
			(*iter)->Disconnect();
		}
		else if (nread < 0) {
			(*iter)->onError(nread,myErrMsg);
			(*iter)->Disconnect();
		}
		if (myClasses.empty()) {
			break;
		}
	}
	if (buf->base != NULL) {
		free(buf->base);
	}
	return;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_shutdown_cb(uv_shutdown_t * req, int state)
{
	if (state < 0) {
		std::vector<TCPAsyncClientSocket*> &myClasses = TCPAsyncClientSocket::m_MyClassPtrs[(uv_tcp_t*)req->handle];
		for (auto iter = myClasses.begin(); iter != myClasses.end(); iter++) {
			(*iter)->onError(state, uv_err_name(state));
			(*iter)->Closing = false;
		}
		free(req);
		return;
	}
	uv_close((uv_handle_t*)req->handle, TCPAsyncClientSocket::m_uv_close_cb);
	free(req);
	return;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_write_cb(uv_write_t * req, int status)
{ //Write Callback
	free(req);
	return;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

//TCPAsyncClientSocketv4 Class
EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::TCPAsyncClientSocket() {
	this->Init();
}

EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::TCPAsyncClientSocket(const IpAddr& RemoteIP){
	this->m_remoteAddr = RemoteIP;
	this->Init();
}
EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::TCPAsyncClientSocket(const TCPAsyncClientSocket & oldClient)
{
	this->Inited = oldClient.Inited;
	this->m_ClientSocketHandle = oldClient.m_ClientSocketHandle;
	this->m_remoteAddr = oldClient.m_remoteAddr;
	this->m_Connected = oldClient.m_Connected;
	this->ConnectCallBack = oldClient.ConnectCallBack;
	this->DisconnectCallBack = oldClient.DisconnectCallBack;
	this->ErrorCallBack = oldClient.ErrorCallBack;
	this->MsgCallBack = oldClient.MsgCallBack;
	this->myMutex = oldClient.myMutex;
	if (oldClient.Inited) {
		SocketParam::m_num_Client++;
		this->m_MyClassPtrs[this->m_ClientSocketHandle.get()].push_back(this);
	}
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::Init()
{
	if (this->Inited) {
		return;
	}
	this->Inited = true;
	this->m_ClientSocketHandle = std::shared_ptr<uv_tcp_t>(new uv_tcp_t);
	//this->m_MyClassPtrs[this->m_ClientSocketHandle.get()] = std::pair<std::mutex*, std::vector<TCPAsyncClientSocketv4*>>(new std::mutex(), std::vector<TCPAsyncClientSocketv4*>());
	this->m_MyClassPtrs[this->m_ClientSocketHandle.get()] = std::vector<TCPAsyncClientSocket*>();
	this->m_MyClassPtrs[this->m_ClientSocketHandle.get()].push_back(this);

	if (SocketParam::m_num_Client == 0) {
		uv_loop_init(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StartJob(NULL, NULL);
	}
	uv_tcp_init(&SocketParam::m_uv_loop, this->m_ClientSocketHandle.get());
	SocketParam::m_num_Client++;
	
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::Connect()
{
	if (!this->Inited) {
		this->Init();
	}
	uv_connect_t *myReadReq = (uv_connect_t*) malloc(sizeof(uv_connect_t));
	sockaddr myAddress = this->m_remoteAddr.getIPAddress();
	uv_tcp_connect(myReadReq, this->m_ClientSocketHandle.get(), &myAddress, EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_connect_cb);
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::setRemoteIPAddr(const IpAddr & newIP)
{
	this->m_remoteAddr = newIP;
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
	return IpAddr();
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
	return IpAddr();
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::SendMsg(const std::string & Msg)
{
	if (!this->m_Connected || !this->Inited) {
		return;
	}
	uv_write_t* myWriteRequest = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_buf_t myWriteBuf;
	
	char* myBufSpace = new char[Msg.length() + 1];
	for (unsigned int i = 0; i < Msg.length(); i++) {
		myBufSpace[i] = Msg[i];
	}
	myBufSpace[Msg.length()] = '\0';
	
	myWriteBuf = uv_buf_init(myBufSpace, Msg.length());
	uv_write(myWriteRequest, (uv_stream_t*)this->m_ClientSocketHandle.get(), &myWriteBuf, 1, TCPAsyncClientSocket::m_uv_write_cb);
	delete[] myBufSpace;
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::Disconnect()
{
	if (!this->m_Connected) {
		return;
	}
	if (this->Inited) {
		this->Closing = true;
		uv_read_stop((uv_stream_t*)this->m_ClientSocketHandle.get());
		uv_shutdown_t* myReq = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
		uv_shutdown(myReq, (uv_stream_t*)this->m_ClientSocketHandle.get(), TCPAsyncClientSocket::m_uv_shutdown_cb);
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
	std::vector<TCPAsyncClientSocket*> &MyVector = m_MyClassPtrs[this->m_ClientSocketHandle.get()];
	
	if (!MyVector.empty()) {
		for (auto MyIt = MyVector.begin(); MyIt != MyVector.end(); MyIt++) {
			if ((*MyIt) == this) {
				MyIt = MyVector.erase(MyIt);
				break;
			}
		}
	}
	if (MyVector.empty()) {
		this->Disconnect();
		this->m_MyClassPtrs.erase(this->m_ClientSocketHandle.get());
	}
	this->m_ClientSocketHandle.reset();
	SocketParam::m_num_Client--;
	if (SocketParam::m_num_Client == 0) {
		uv_stop(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StopJob();
		uv_loop_close(&SocketParam::m_uv_loop);
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::onConnected(bool Succeeded)
{
	if (this->ConnectCallBack != NULL) {
		this->ConnectCallBack(Succeeded,(void*) this);
	}
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::onMsg(const std::string & Msg)
{
	if (this->MsgCallBack != NULL) {
		this->MsgCallBack(Msg,(void*) this);
	}
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::onDisconnect()
{
	if (this->DisconnectCallBack != NULL) {
		this->DisconnectCallBack((void*) this);
	}
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::onError(int errCode, const std::string& errDescription)
{
	if (this->ErrorCallBack != NULL) {
		this->ErrorCallBack(errCode, errDescription,(void*) this);
	}
}
EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::~TCPAsyncClientSocket()
{
	this->Destroy();
}
//TCPAsyncClientSocketv4 Class Ends

std::map<uv_tcp_t*, std::vector<EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket*>> EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::m_MyClassPtrs;
void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::onConnection(TCPAsyncClientSocket* newClient)
{
	newClient->onConnected(true);
	if (this->ServerNewConnCallBack != NULL) {
		this->ServerNewConnCallBack(newClient, (void*) this);
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::onError(int errCode, const std::string & errorDescription)
{
	if (this->ServerErrorCallBack != NULL) {
		this->ServerErrorCallBack(errCode, errorDescription, (void*) this);
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::m_uv_connection_cb(uv_stream_t * server, int status)
{
	std::vector<TCPAsyncServerSocket*> &MyClasses = TCPAsyncServerSocket::m_MyClassPtrs[(uv_tcp_t*)server];
	TCPAsyncClientSocket* MyNewClient = NULL;
	int acceptState = 0;
	if (status == 0) {
		//Succeed!
		MyNewClient = new TCPAsyncClientSocket();
		MyNewClient->Init();
		acceptState = uv_accept(server, (uv_stream_t*) MyNewClient->m_ClientSocketHandle.get());
		if (acceptState == 0) {
			uv_read_start((uv_stream_t*)MyNewClient->m_ClientSocketHandle.get(), TCPAsyncClientSocket::m_uv_alloc_buffer, TCPAsyncClientSocket::m_uv_read_cb);
			MyNewClient->m_Connected = true;
		}
		else {
			MyNewClient->m_Connected = false;
		}
	}
	for (auto iter = MyClasses.begin(); iter != MyClasses.end(); iter++) {
		if (status < 0) {
			(*iter)->onError(status, uv_err_name(status));
			continue;
		}
		if (acceptState != 0) {
			(*iter)->onError(acceptState, uv_err_name(acceptState));
			continue;
		}
		if ((*iter)->isListening) {
			MyNewClient->ConnectCallBack = (*iter)->ClientConnectCallBack;
			MyNewClient->DisconnectCallBack = (*iter)->ClientDisconnectCallBack;
			MyNewClient->ErrorCallBack = (*iter)->ClientErrorCallBack;
			MyNewClient->MsgCallBack = (*iter)->ClientMsgCallBack;
			SocketParam::m_num_Client++;
			(*iter)->onConnection(MyNewClient);
		}
	}
	if (acceptState != 0 && MyNewClient != NULL) {
		delete MyNewClient;
	}
}

EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::TCPAsyncServerSocket()
{
	this->Init();
}

EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::TCPAsyncServerSocket(const IpAddr & myIP, int QueLength)
{
	this->m_myIP = myIP;
	this->m_QueueLength = QueLength;
	this->Init();
}

EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::TCPAsyncServerSocket(const TCPAsyncServerSocket & oldServer)
{
	
	this->m_SocketHandle = oldServer.m_SocketHandle;
	this->m_QueueLength = oldServer.m_QueueLength;
	this->isListening = oldServer.isListening;
	this->ServerErrorCallBack = oldServer.ServerErrorCallBack;
	this->ServerNewConnCallBack = oldServer.ServerNewConnCallBack;
	this->ClientConnectCallBack = oldServer.ClientConnectCallBack;
	this->ClientDisconnectCallBack = oldServer.ClientDisconnectCallBack;
	this->ClientErrorCallBack = oldServer.ClientErrorCallBack;
	this->ClientMsgCallBack = oldServer.ClientMsgCallBack;
	this->hasInted = oldServer.hasInted;
	if (oldServer.hasInted) {
		SocketParam::m_num_Client++;
		this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::Init()
{
	if (this->hasInted) {
		return;
	}
	this->m_SocketHandle = std::shared_ptr<uv_tcp_t>(new uv_tcp_t);
	this->m_MyClassPtrs[this->m_SocketHandle.get()] = std::vector<TCPAsyncServerSocket*>();
	this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
	if (SocketParam::m_num_Client == 0) {
		uv_loop_init(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StartJob(NULL, NULL);
	}
	uv_tcp_init(&SocketParam::m_uv_loop, this->m_SocketHandle.get());
	SocketParam::m_num_Client++;
	this->hasInted = true;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::Destroy()
{
	if (!this->hasInted) {
		return;
	}
	this->hasInted = false;
	std::vector<TCPAsyncServerSocket*> &MyVector = this->m_MyClassPtrs[this->m_SocketHandle.get()];

	for (auto MyIt = MyVector.begin(); MyIt != MyVector.end(); MyIt++) {
		if ((*MyIt) == this) {
			MyIt = MyVector.erase(MyIt);
			if (MyVector.empty()) {
				break;
			}
		}
	}
	if (MyVector.empty()) {
		if (this->isListening) {
			this->StopListen();
		}
		this->m_MyClassPtrs.erase(this->m_SocketHandle.get());
	}

	SocketParam::m_num_Client--;
	if (SocketParam::m_num_Client == 0) {
		uv_stop(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StopJob();
		uv_loop_close(&SocketParam::m_uv_loop);
	}
	this->m_SocketHandle.reset();
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
	sockaddr myAddr = this->m_myIP.getIPAddress();
	int bindState = uv_tcp_bind(this->m_SocketHandle.get(), &myAddr, 0);
	if (bindState < 0) {
		this->onError(bindState, uv_err_name(bindState));
		return;
	}
	int listenState = uv_listen((uv_stream_t*)this->m_SocketHandle.get(), this->m_QueueLength, TCPAsyncServerSocket::m_uv_connection_cb);
	if (listenState < 0) {
		this->onError(listenState, uv_err_name(listenState));
		return;
	}
	this->isListening = true;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::Listen(const IpAddr & myIP, int QueLength)
{
	this->m_myIP = myIP;
	this->m_QueueLength = QueLength;
	this->Listen();
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::StopListen()
{
	this->isListening = false;
	uv_close((uv_handle_t*) this->m_SocketHandle.get(), NULL);
}

EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::~TCPAsyncServerSocket()
{
	this->Destroy();
}
