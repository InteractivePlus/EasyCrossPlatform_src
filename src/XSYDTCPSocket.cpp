#include <XSYDTCPSocket.h>

std::map<EasyCrossPlatform::Network::Socket::SocketWorker*,std::map<uv_tcp_t*, std::vector<EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*>>> EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_MyClassPtrs;
std::mutex EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::sharedMutex;
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_connect_cb(uv_connect_t * req, int status)
{
	uv_tcp_t* mytcpHandle = (uv_tcp_t*)req->handle;
	free(req);
	std::vector<TCPAsyncClientSocket*> &myClasses = TCPAsyncClientSocket::m_MyClassPtrs[(SocketWorker*) req->data][mytcpHandle];
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
	std::vector<TCPAsyncClientSocket*> MyClasses = TCPAsyncClientSocket::m_MyClassPtrs[(SocketWorker*) handle->data][((uv_tcp_t*)handle)];

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
	std::vector<TCPAsyncClientSocket*> &myClasses = TCPAsyncClientSocket::m_MyClassPtrs[(SocketWorker*) stream->data][(uv_tcp_t*)stream];
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
		std::vector<TCPAsyncClientSocket*> &myClasses = TCPAsyncClientSocket::m_MyClassPtrs[(SocketWorker*)req->data][(uv_tcp_t*)req->handle];
		for (auto iter = myClasses.begin(); iter != myClasses.end(); iter++) {
			(*iter)->onError(state, uv_err_name(state));
			(*iter)->Closing = false;
		}
		free(req);
		return;
	}
	free(req);
	return;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_write_cb(uv_write_t * req, int status)
{
	free(req);
	return;
}

void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::m_uv_alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::TCPAsyncClientSocket() {
	
}

EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::TCPAsyncClientSocket(const IpAddr& RemoteIP){
	this->m_remoteAddr = RemoteIP;
	
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
	this->mySocketWorker = oldClient.mySocketWorker;
	if (oldClient.Inited) {
		this->mySocketWorker->m_num_Client++;
		this->m_MyClassPtrs[this->mySocketWorker][this->m_ClientSocketHandle.get()].push_back(this);
	}
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
	this->m_ClientSocketHandle.get()->data = (void*)this->mySocketWorker;
	if (this->m_MyClassPtrs.find(this->mySocketWorker) == this->m_MyClassPtrs.end()) {
		this->m_MyClassPtrs[this->mySocketWorker] = std::map<uv_tcp_t*, std::vector<TCPAsyncClientSocket*>>();
	}
	this->m_MyClassPtrs[this->mySocketWorker][this->m_ClientSocketHandle.get()] = std::vector<TCPAsyncClientSocket*>();
	this->m_MyClassPtrs[this->mySocketWorker][this->m_ClientSocketHandle.get()].push_back(this);

	if (this->mySocketWorker->m_num_Client == 0) {
		uv_loop_init(this->mySocketWorker->m_uv_loop.get());
		this->mySocketWorker->Start();
	}
	uv_tcp_init(this->mySocketWorker->m_uv_loop.get(), this->m_ClientSocketHandle.get());
	this->mySocketWorker->m_num_Client++;
	
}
void EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket::Connect()
{
	if (!this->Inited) {
		this->onError(-1, "You cannot connect without intializing");
		return;
	}
	uv_connect_t *myReadReq = (uv_connect_t*) malloc(sizeof(uv_connect_t));
	myReadReq->data = (void*)this->mySocketWorker;
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
	myWriteRequest->data = (void*)this->mySocketWorker;
	
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
	std::vector<TCPAsyncClientSocket*> &MyVector = m_MyClassPtrs[this->mySocketWorker][this->m_ClientSocketHandle.get()];
	
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
		this->sharedMutex.lock();
			this->m_MyClassPtrs[this->mySocketWorker].erase(this->m_ClientSocketHandle.get());
		this->sharedMutex.unlock();
	}
	if (this->m_MyClassPtrs[this->mySocketWorker].empty()) {
		this->m_MyClassPtrs.erase(this->mySocketWorker);
	}
	this->m_ClientSocketHandle.reset();
	this->mySocketWorker->m_num_Client--;
	if (this->mySocketWorker->m_num_Client == 0) {
		uv_stop(this->mySocketWorker->m_uv_loop.get());
		this->mySocketWorker->Stop();
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
	for (auto iter = MyClasses.begin(); iter != MyClasses.end(); iter++) {
		if (status < 0) {
			(*iter)->onError(status, uv_err_name(status));
			continue;
		}
		else if (acceptState != 0) {
			
		}
		else if ((*iter)->isListening) {
			MyNewClient = new TCPAsyncClientSocket();
			MyNewClient->setWorker((*iter)->myClientWorker.front());
			(*iter)->myClientWorker.push_back((*iter)->myClientWorker.front());
			(*iter)->myClientWorker.pop_front();
			MyNewClient->Init();
			acceptState = uv_accept(server, (uv_stream_t*)MyNewClient->m_ClientSocketHandle.get());
			if (acceptState == 0) {
				uv_read_start((uv_stream_t*)MyNewClient->m_ClientSocketHandle.get(), TCPAsyncClientSocket::m_uv_alloc_buffer, TCPAsyncClientSocket::m_uv_read_cb);
				MyNewClient->m_Connected = true;
			}
			else {
				MyNewClient->m_Connected = false;
				(*iter)->onError(acceptState, uv_err_name(acceptState));
				delete MyNewClient;
				continue;
			}
			MyNewClient->ConnectCallBack = (*iter)->ClientConnectCallBack;
			MyNewClient->DisconnectCallBack = (*iter)->ClientDisconnectCallBack;
			MyNewClient->ErrorCallBack = (*iter)->ClientErrorCallBack;
			MyNewClient->MsgCallBack = (*iter)->ClientMsgCallBack;
			(*iter)->onConnection(MyNewClient);
		}
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
	
	this->m_SocketHandle = oldServer.m_SocketHandle;
	this->m_QueueLength = oldServer.m_QueueLength;
	this->isListening = oldServer.isListening;
	this->ServerErrorCallBack = oldServer.ServerErrorCallBack;
	this->ServerNewConnCallBack = oldServer.ServerNewConnCallBack;
	this->ClientConnectCallBack = oldServer.ClientConnectCallBack;
	this->ClientDisconnectCallBack = oldServer.ClientDisconnectCallBack;
	this->ClientErrorCallBack = oldServer.ClientErrorCallBack;
	this->ClientMsgCallBack = oldServer.ClientMsgCallBack;
	this->myListenWorker = oldServer.myListenWorker;
	this->myClientWorker = oldServer.myClientWorker;
	this->hasInted = oldServer.hasInted;
	if (oldServer.hasInted) {
		this->myListenWorker->m_num_Client++;
		this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
	}
}

void EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::setWorkers(SocketWorker* listeningWorker, std::deque<SocketWorker*>& clientWorkers)
{
	this->myListenWorker = listeningWorker;
	this->myClientWorker = clientWorkers;
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
	this->m_SocketHandle = std::shared_ptr<uv_tcp_t>(new uv_tcp_t);
	this->m_MyClassPtrs[this->m_SocketHandle.get()] = std::vector<TCPAsyncServerSocket*>();
	this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
	if (this->myListenWorker->m_num_Client == 0) {
		uv_loop_init(this->myListenWorker->m_uv_loop.get());
		this->myListenWorker->Start();
	}
	uv_tcp_init(this->myListenWorker->m_uv_loop.get(), this->m_SocketHandle.get());
	this->myListenWorker->m_num_Client++;
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

	this->myListenWorker->m_num_Client--;
	if (this->myListenWorker->m_num_Client == 0) {
		uv_stop(this->myListenWorker->m_uv_loop.get());
		this->myListenWorker->Stop();
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
	if (!this->hasInted) {
		this->onError(-1, "You cannot Listen on your socket without Intializing");
		return;
	}
	else if(this->m_QueueLength == 0){
		this->onError(-1, "You havent specified your IP Address to listen, and your queue length");
		return;
	}
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
	if (this->hasInted) {
		for (auto i = this->myClientWorker.begin(); i != this->myClientWorker.end(); i++) {
			uv_stop((*i)->m_uv_loop.get());
			(*i)->Stop();
		}
	}
	uv_close((uv_handle_t*) this->m_SocketHandle.get(), NULL);
}

EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket::~TCPAsyncServerSocket()
{
	this->Destroy();
}
