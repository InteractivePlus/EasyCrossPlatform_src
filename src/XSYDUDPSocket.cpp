#include <XSYDUDPSocket.h>
std::map<uv_udp_t*, std::vector<EasyCrossPlatform::Network::Socket::UDPAsyncSocket*>> EasyCrossPlatform::Network::Socket::UDPAsyncSocket::m_MyClassPtrs;
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::m_uv_read_cb(uv_udp_t * handle, ssize_t nread, const uv_buf_t * buf, const sockaddr * remoteaddr, unsigned flags)
{
	std::vector<UDPAsyncSocket*> MyClasses = UDPAsyncSocket::m_MyClassPtrs[handle];
	std::string errMsg;
	if (nread < 0) {
		errMsg = uv_err_name(nread);
	}
	std::string data;
	IpAddr RemoteAddr;
	if (remoteaddr != NULL) {
		RemoteAddr.m_Addr = (*(remoteaddr));
	}
	if (nread > 0) {
		data.assign(buf->base, nread);
	}
	for (auto iter = MyClasses.begin(); iter != MyClasses.end(); iter++) {
		if (nread < 0){
			(*iter)->onError(nread, errMsg);
		}
		else if (nread > 0) {
			(*iter)->onMsg(RemoteAddr, data);
		}
	}
	if (buf->base) {
		free(buf->base);
	}
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::m_uv_send_cb(uv_udp_send_t * req, int status)
{
	std::vector<UDPAsyncSocket*> &MyClasses = UDPAsyncSocket::m_MyClassPtrs[(uv_udp_t*)req->handle];
	for (auto iter = MyClasses.begin(); iter != MyClasses.end(); iter++) {
		if (status < 0) {
			(*iter)->onError(status, uv_err_name(status));
		}
	}
	free(req);
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::m_uv_alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}
EasyCrossPlatform::Network::Socket::UDPAsyncSocket::UDPAsyncSocket()
{
	this->Init();
}
EasyCrossPlatform::Network::Socket::UDPAsyncSocket::UDPAsyncSocket(const IpAddr& ListeningAddr)
{
	this->m_isListenMode = true;
	this->m_myAddr = ListeningAddr;
	this->Init();
}
EasyCrossPlatform::Network::Socket::UDPAsyncSocket::UDPAsyncSocket(UDPAsyncSocket & oldUDP)
{
	this->m_isListening = oldUDP.m_isListening;
	this->m_isListenMode = oldUDP.m_isListenMode;
	this->m_SocketHandle = oldUDP.m_SocketHandle;
	this->m_hasInited = oldUDP.m_hasInited;
	this->mySocketWorker = oldUDP.mySocketWorker;
	if (oldUDP.m_hasInited) {
		this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
		this->mySocketWorker->m_num_Client++;
	}
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::setmyIP(const IpAddr& myNewIP)
{
	this->m_myAddr = myNewIP;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::sendMsg(const IpAddr& Destination, const std::string& Data)
{
	uv_udp_send_t* mySendReq = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
	IpAddr MyDestination = Destination;
	sockaddr myAddr = MyDestination.getIPAddress();
	char* tmpData = new char[Data.length() + 1];
	for (unsigned int i = 0; i < Data.length(); i++) {
		tmpData[i] = Data[i];
	}
	tmpData[Data.length()] = '\0';
	uv_buf_t myBuffer = uv_buf_init(tmpData, Data.length());
	
	uv_udp_send(mySendReq, this->m_SocketHandle.get(), &myBuffer, 1, &myAddr, UDPAsyncSocket::m_uv_send_cb);
	delete[] tmpData;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::Listen()
{
	if (!this->m_isListenMode) {
		return;
	}
	sockaddr myAddress = this->m_myAddr.getIPAddress();

	int state = uv_udp_bind(this->m_SocketHandle.get(), &myAddress, 0);
	if (state < 0) {
		std::string errorMsg;
		errorMsg = uv_err_name(state);
		this->onError(state, errorMsg);
		return;
	}
	int recvState = uv_udp_recv_start(this->m_SocketHandle.get(), UDPAsyncSocket::m_uv_alloc_buffer, UDPAsyncSocket::m_uv_read_cb);
	if (recvState < 0) {
		this->onError(recvState, uv_err_name(recvState));
		return;
	}
	this->m_isListening = true;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::Listen(const IpAddr& myNewAddr)
{
	this->m_isListenMode = true;
	this->m_myAddr = myNewAddr;
	this->Listen();
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::StopListen()
{
	if (this->m_isListenMode && this->m_isListening) {
		uv_udp_recv_stop(this->m_SocketHandle.get());
		this->m_isListening = false;
	}
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::setWorker(SocketWorker & myWorker)
{
	this->mySocketWorker = &myWorker;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::Init()
{
	if (this->m_hasInited) {
		return;
	}
	else if (this->mySocketWorker == NULL) {
		return;
	}
	this->m_SocketHandle = std::shared_ptr<uv_udp_t>(new uv_udp_t);
	if (this->mySocketWorker->m_num_Client == 0) {
		uv_loop_init(this->mySocketWorker->m_uv_loop.get());
		this->mySocketWorker->Start();
	}
	this->m_MyClassPtrs[this->m_SocketHandle.get()] = std::vector<UDPAsyncSocket*>();
	this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
	this->mySocketWorker->m_num_Client++;
	uv_udp_init(this->mySocketWorker->m_uv_loop.get(), this->m_SocketHandle.get());
	this->m_hasInited = true;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::Destroy()
{
	if (!this->m_hasInited) {
		return;
	}
	
	std::vector<UDPAsyncSocket*> &myVector = this->m_MyClassPtrs[this->m_SocketHandle.get()];
	for (auto iter = myVector.begin(); iter != myVector.end(); iter++) {
		if ((*iter) == this) {
			iter = myVector.erase(iter);
			break;
		}
	}
	if (myVector.empty()) {
		if (this->m_isListening && this->m_isListenMode) {
			this->StopListen();
		}
		uv_close((uv_handle_t*)this->m_SocketHandle.get(), NULL);
		this->m_MyClassPtrs.erase(this->m_SocketHandle.get());
	}
	this->mySocketWorker->m_num_Client--;
	if (this->mySocketWorker->m_num_Client == 0) {
		uv_stop(this->mySocketWorker->m_uv_loop.get());
		this->mySocketWorker->Stop();
		uv_loop_close(this->mySocketWorker->m_uv_loop.get());
	}
	this->m_hasInited = false;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::onMsg(const IpAddr& SourceIP, const std::string& data)
{
	return;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocket::onError(int errCode, const std::string& errDescription)
{
	return;
}

EasyCrossPlatform::Network::Socket::UDPAsyncSocket::~UDPAsyncSocket()
{
	this->Destroy();
}