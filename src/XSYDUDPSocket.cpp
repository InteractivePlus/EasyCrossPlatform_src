#include <XSYDUDPSocket.h>
std::map<uv_udp_t*, std::vector<EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4*>> EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::m_MyClassPtrs;
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::m_uv_read_cb(uv_udp_t * handle, ssize_t nread, const uv_buf_t * buf, const sockaddr * remoteaddr, unsigned flags)
{
	std::vector<UDPAsyncSocketv4*> MyClasses = UDPAsyncSocketv4::m_MyClassPtrs[handle];
	std::string errMsg;
	if (nread < 0) {
		errMsg = uv_err_name(nread);
	}
	std::string data;
	IpAddrV4 RemoteAddr;
	if (remoteaddr != NULL) {
		RemoteAddr.m_Addr = (*((sockaddr_in*)remoteaddr));
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
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::m_uv_send_cb(uv_udp_send_t * req, int status)
{
	std::vector<UDPAsyncSocketv4*> &MyClasses = UDPAsyncSocketv4::m_MyClassPtrs[(uv_udp_t*)req->handle];
	for (auto iter = MyClasses.begin(); iter != MyClasses.end(); iter++) {
		if (status < 0) {
			(*iter)->onError(status, uv_err_name(status));
		}
	}
	free(req);
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::m_uv_alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}
EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::UDPAsyncSocketv4()
{
	this->Init();
}
EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::UDPAsyncSocketv4(const IpAddrV4& ListeningAddr)
{
	this->m_isListenMode = true;
	this->m_myAddr = ListeningAddr;
	this->Init();
}
EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::UDPAsyncSocketv4(UDPAsyncSocketv4 & oldUDP)
{
	this->m_isListening = oldUDP.m_isListening;
	this->m_isListenMode = oldUDP.m_isListenMode;
	this->m_SocketHandle = oldUDP.m_SocketHandle;
	this->m_hasInited = oldUDP.m_hasInited;

	if (oldUDP.m_hasInited) {
		this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
		SocketParam::m_num_Client++;
	}
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::setmyIP(const IpAddrV4& myNewIP)
{
	this->m_myAddr = myNewIP;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::sendMsg(const IpAddrV4& Destination, const std::string& Data)
{
	uv_udp_send_t* mySendReq = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
	IpAddrV4 MyDestination = Destination;
	sockaddr_in myAddr = MyDestination.getIPAddress();
	char* tmpData = new char[Data.length() + 1];
	for (unsigned int i = 0; i < Data.length(); i++) {
		tmpData[i] = Data[i];
	}
	tmpData[Data.length()] = '\0';
	uv_buf_t myBuffer = uv_buf_init(tmpData, Data.length());
	
	uv_udp_send(mySendReq, this->m_SocketHandle.get(), &myBuffer, 1, (sockaddr*) &myAddr, UDPAsyncSocketv4::m_uv_send_cb);
	delete[] tmpData;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::Listen()
{
	if (!this->m_isListenMode) {
		return;
	}
	sockaddr_in myAddress = this->m_myAddr.getIPAddress();

	int state = uv_udp_bind(this->m_SocketHandle.get(), (sockaddr*)&myAddress, 0);
	if (state < 0) {
		std::string errorMsg;
		errorMsg = uv_err_name(state);
		this->onError(state, errorMsg);
		return;
	}
	int recvState = uv_udp_recv_start(this->m_SocketHandle.get(), UDPAsyncSocketv4::m_uv_alloc_buffer, UDPAsyncSocketv4::m_uv_read_cb);
	if (recvState < 0) {
		this->onError(recvState, uv_err_name(recvState));
		return;
	}
	this->m_isListening = true;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::Listen(const IpAddrV4& myNewAddr)
{
	this->m_isListenMode = true;
	this->m_myAddr = myNewAddr;
	this->Listen();
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::StopListen()
{
	if (this->m_isListenMode && this->m_isListening) {
		uv_udp_recv_stop(this->m_SocketHandle.get());
		this->m_isListening = false;
	}
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::Init()
{
	if (this->m_hasInited) {
		return;
	}
	this->m_SocketHandle = std::shared_ptr<uv_udp_t>(new uv_udp_t);
	if (SocketParam::m_num_Client == 0) {
		uv_loop_init(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StartJob();
	}
	this->m_MyClassPtrs[this->m_SocketHandle.get()] = std::vector<UDPAsyncSocketv4*>();
	this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
	SocketParam::m_num_Client++;
	uv_udp_init(&SocketParam::m_uv_loop, this->m_SocketHandle.get());
	this->m_hasInited = true;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::Destroy()
{
	if (!this->m_hasInited) {
		return;
	}
	
	std::vector<UDPAsyncSocketv4*> &myVector = this->m_MyClassPtrs[this->m_SocketHandle.get()];
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
	SocketParam::m_num_Client--;
	if (SocketParam::m_num_Client == 0) {
		uv_stop(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StopJob();
		uv_loop_close(&SocketParam::m_uv_loop);
	}
	this->m_hasInited = false;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::onMsg(const IpAddrV4& SourceIP, const std::string& data)
{
	return;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::onError(int errCode, const std::string& errDescription)
{
	return;
}

EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4::~UDPAsyncSocketv4()
{
	this->Destroy();
}


std::map<uv_udp_t*, std::vector<EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6*>> EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::m_MyClassPtrs;
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::m_uv_read_cb(uv_udp_t * handle, ssize_t nread, const uv_buf_t * buf, const sockaddr * remoteaddr, unsigned flags)
{
	std::vector<UDPAsyncSocketv6*> MyClasses = UDPAsyncSocketv6::m_MyClassPtrs[handle];
	std::string errMsg;
	if (nread < 0) {
		errMsg = uv_err_name(nread);
	}
	std::string data;
	IpAddrV6 RemoteAddr;
	if (remoteaddr != NULL) {
		RemoteAddr.m_Addr = (*((sockaddr_in6*)remoteaddr));
	}
	if (nread > 0) {
		data.assign(buf->base, nread);
	}
	for (auto iter = MyClasses.begin(); iter != MyClasses.end(); iter++) {
		if (nread < 0) {
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
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::m_uv_send_cb(uv_udp_send_t * req, int status)
{
	std::vector<UDPAsyncSocketv6*> &MyClasses = UDPAsyncSocketv6::m_MyClassPtrs[(uv_udp_t*)req->handle];
	for (auto iter = MyClasses.begin(); iter != MyClasses.end(); iter++) {
		if (status < 0) {
			(*iter)->onError(status, uv_err_name(status));
		}
	}
	free(req);
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::m_uv_alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}
EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::UDPAsyncSocketv6()
{
	this->Init();
}
EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::UDPAsyncSocketv6(const IpAddrV6& ListeningAddr)
{
	this->m_isListenMode = true;
	this->m_myAddr = ListeningAddr;
	this->Init();
}
EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::UDPAsyncSocketv6(UDPAsyncSocketv6 & oldUDP)
{
	this->m_isListening = oldUDP.m_isListening;
	this->m_isListenMode = oldUDP.m_isListenMode;
	this->m_SocketHandle = oldUDP.m_SocketHandle;
	this->m_hasInited = oldUDP.m_hasInited;

	if (oldUDP.m_hasInited) {
		this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
		SocketParam::m_num_Client++;
	}
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::setmyIP(const IpAddrV6& myNewIP)
{
	this->m_myAddr = myNewIP;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::sendMsg(const IpAddrV6& Destination, const std::string& Data)
{
	uv_udp_send_t* mySendReq = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
	IpAddrV6 MyDestination = Destination;
	sockaddr_in6 myAddr = MyDestination.getIPAddress();
	char* tmpData = new char[Data.length() + 1];
	for (unsigned int i = 0; i < Data.length(); i++) {
		tmpData[i] = Data[i];
	}
	tmpData[Data.length()] = '\0';
	uv_buf_t myBuffer = uv_buf_init(tmpData, Data.length());

	uv_udp_send(mySendReq, this->m_SocketHandle.get(), &myBuffer, 1, (sockaddr*)&myAddr, UDPAsyncSocketv6::m_uv_send_cb);
	delete[] tmpData;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::Listen()
{
	if (!this->m_isListenMode) {
		return;
	}
	sockaddr_in6 myAddress = this->m_myAddr.getIPAddress();

	int state = uv_udp_bind(this->m_SocketHandle.get(), (sockaddr*)&myAddress, 0);
	if (state < 0) {
		std::string errorMsg;
		errorMsg = uv_err_name(state);
		this->onError(state, errorMsg);
		return;
	}
	int recvState = uv_udp_recv_start(this->m_SocketHandle.get(), UDPAsyncSocketv6::m_uv_alloc_buffer, UDPAsyncSocketv6::m_uv_read_cb);
	if (recvState < 0) {
		this->onError(recvState, uv_err_name(recvState));
		return;
	}
	this->m_isListening = true;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::Listen(const IpAddrV6& myNewAddr)
{
	this->m_isListenMode = true;
	this->m_myAddr = myNewAddr;
	this->Listen();
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::StopListen()
{
	if (this->m_isListenMode && this->m_isListening) {
		uv_udp_recv_stop(this->m_SocketHandle.get());
		this->m_isListening = false;
	}
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::Init()
{
	if (this->m_hasInited) {
		return;
	}
	this->m_SocketHandle = std::shared_ptr<uv_udp_t>(new uv_udp_t);
	if (SocketParam::m_num_Client == 0) {
		uv_loop_init(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StartJob();
	}
	this->m_MyClassPtrs[this->m_SocketHandle.get()] = std::vector<UDPAsyncSocketv6*>();
	this->m_MyClassPtrs[this->m_SocketHandle.get()].push_back(this);
	SocketParam::m_num_Client++;
	uv_udp_init(&SocketParam::m_uv_loop, this->m_SocketHandle.get());
	this->m_hasInited = true;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::Destroy()
{
	if (!this->m_hasInited) {
		return;
	}

	std::vector<UDPAsyncSocketv6*> &myVector = this->m_MyClassPtrs[this->m_SocketHandle.get()];
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
	SocketParam::m_num_Client--;
	if (SocketParam::m_num_Client == 0) {
		uv_stop(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StopJob();
		uv_loop_close(&SocketParam::m_uv_loop);
	}
	this->m_hasInited = false;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::onMsg(const IpAddrV6& SourceIP, const std::string& data)
{
	return;
}
void EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::onError(int errCode, const std::string& errDescription)
{
	return;
}

EasyCrossPlatform::Network::Socket::UDPAsyncSocketv6::~UDPAsyncSocketv6()
{
	this->Destroy();
}
