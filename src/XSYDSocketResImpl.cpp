#include <XSYDSocketResImpl.h>

uv_loop_t EasyCrossPlatform::Network::Socket::SocketParam::m_uv_loop;
unsigned int EasyCrossPlatform::Network::Socket::SocketParam::m_num_Client = 0U;
EasyCrossPlatform::Thread::SingleWork EasyCrossPlatform::Network::Socket::SocketParam::m_MTManager = EasyCrossPlatform::Thread::SingleWork(SocketParam::m_MultiThread_Job);

void EasyCrossPlatform::Network::Socket::SocketParam::Start()
{
	uv_loop_init(&SocketParam::m_uv_loop);
	SocketParam::m_MTManager.StartJob(NULL, NULL);
}

void EasyCrossPlatform::Network::Socket::SocketParam::Stop()
{
	uv_stop(&SocketParam::m_uv_loop);
	SocketParam::m_MTManager.StopJob();
}

void EasyCrossPlatform::Network::Socket::SocketParam::m_MultiThread_Job(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	while ((*RunningSign)) {
		uv_run(&SocketParam::m_uv_loop, UV_RUN_DEFAULT);
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
	}
	uv_loop_close(&SocketParam::m_uv_loop);
	return;
}


EasyCrossPlatform::Network::Socket::IpAddr::IpAddr()
{
	this->m_isIpV4 = true;
}

EasyCrossPlatform::Network::Socket::IpAddr::IpAddr(const std::string & IpAddress, const unsigned short Port)
{
	this->setIPAddress(IpAddress, Port);
}

EasyCrossPlatform::Network::Socket::IpAddr::IpAddr(const char * IpAddress, const unsigned short Port)
{
	this->setIPAddress(std::string(IpAddress), Port);
}

EasyCrossPlatform::Network::Socket::IpAddr::IpAddr(const sockaddr * newAddr)
{
	this->setIPAddress(newAddr);
}

EasyCrossPlatform::Network::Socket::IpAddr::IpAddr(const IpAddr & oldAddr)
{
	this->m_isIpV4 = oldAddr.m_isIpV4;
	this->m_v4Addr = oldAddr.m_v4Addr;
	this->m_v6Addr = oldAddr.m_v6Addr;
}

bool EasyCrossPlatform::Network::Socket::IpAddr::setIPAddress(const std::string & IpAddress, const unsigned short Port)
{
	int setState = 0;
	size_t FindPos = IpAddress.find('.', 0U);
	bool isIPv4 = FindPos != std::string::npos;
	if (isIPv4) {
		setState = uv_ip4_addr(IpAddress.c_str(), static_cast<int>(Port), &this->m_v4Addr);
	}
	else {
		setState = uv_ip6_addr(IpAddress.c_str(), static_cast<int>(Port), &this->m_v6Addr);
	}
	if (setState < 0) {
		return false;
	}
	else {
		this->m_isIpV4 = isIPv4;
		return true;
	}
}

bool EasyCrossPlatform::Network::Socket::IpAddr::setIPAddress(const char * IpAddress, const unsigned short Port)
{
	return setIPAddress(std::string(IpAddress), Port);
}

bool EasyCrossPlatform::Network::Socket::IpAddr::setIPAddress(const sockaddr* newAddr) {
	sockaddr* newAddrAdd = (sockaddr*) newAddr;
	if (newAddr->sa_family == AF_INET) {
		this->m_isIpV4 = true;
		this->m_v4Addr = (* ((sockaddr_in*) newAddrAdd));
	}
	else if (newAddr->sa_family == AF_INET6) {
		this->m_isIpV4 = false;
		this->m_v6Addr = (* ((sockaddr_in6*) newAddrAdd));
	}
	else {
		return false;
	}
	
	return true;
}

std::string EasyCrossPlatform::Network::Socket::IpAddr::getIPString()
{
	if (this->m_isIpV4) {
		char myNewAddr[16] = "";
		memset(myNewAddr, 0, 16);
		int State = uv_ip4_name((sockaddr_in*)&(this->m_v4Addr), (char*)myNewAddr, 16);
		return std::string(myNewAddr);
	}
	else {
		char myNewAddr[40] = "";
		memset(myNewAddr, 0, 40);
		int State = uv_ip6_name((sockaddr_in6*) &(this->m_v6Addr), (char*)myNewAddr, 40);
		return std::string(myNewAddr);
	}
}

bool EasyCrossPlatform::Network::Socket::IpAddr::addrIsIPV4()
{
	return this->m_isIpV4;
}

unsigned short EasyCrossPlatform::Network::Socket::IpAddr::getPort()
{
	if (this->m_isIpV4) {
		sockaddr_in* mySocketAddr = &this->m_v4Addr;
		return ntohs(mySocketAddr->sin_port);
	}
	else {
		sockaddr_in6* mySocketAddr = &this->m_v6Addr;
		return ntohs(mySocketAddr->sin6_port);
	}
}

const sockaddr* EasyCrossPlatform::Network::Socket::IpAddr::getIPAddress()
{
	sockaddr* resultaddr;
	if (this->m_isIpV4) {
		resultaddr = (sockaddr*) &this->m_v4Addr;
	}
	else {
		resultaddr = (sockaddr*) &this->m_v6Addr;
	}
	return resultaddr;
}

EasyCrossPlatform::Network::Socket::SocketWorker::SocketWorker()
{
	this->m_uv_loop = std::shared_ptr<uv_loop_t>(new uv_loop_t());
}

EasyCrossPlatform::Network::Socket::SocketWorker::SocketWorker(SocketWorker & LeftSocket)
{
	throw std::runtime_error("233");
}

void EasyCrossPlatform::Network::Socket::SocketWorker::Start()
{
	uv_loop_init(this->m_uv_loop.get());
	this->m_MTManager.setWork(SocketWorker::m_MultiThread_Job);
	this->m_MTManager.StartJob(NULL, (void*)this);
}

void EasyCrossPlatform::Network::Socket::SocketWorker::Stop()
{
	uv_stop(this->m_uv_loop.get());
	this->m_MTManager.StopJob();
}

void EasyCrossPlatform::Network::Socket::SocketWorker::Increment()
{
	this->m_num_Client++;
	if (this->m_num_Client == 1) {
		this->Start();
	}
}

void EasyCrossPlatform::Network::Socket::SocketWorker::Decrement()
{
	this->m_num_Client--;
	if (this->m_num_Client == 0) {
		this->Stop();
	}
}

void EasyCrossPlatform::Network::Socket::SocketWorker::m_MultiThread_Job(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	SocketWorker* myWorker = (SocketWorker*)Parameters;
	while ((*RunningSign)) {
		uv_run(myWorker->m_uv_loop.get(), UV_RUN_DEFAULT);
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
	}
	uv_loop_close(myWorker->m_uv_loop.get());
	return;
}

EasyCrossPlatform::Network::Socket::SocketWorker::~SocketWorker()
{
	this->Stop();
}

void * EasyCrossPlatform::Network::Socket::CustomDataCarrier::getCustomData()
{
	return this->CustomData;
}

void EasyCrossPlatform::Network::Socket::CustomDataCarrier::setCustomData(void * Data)
{
	this->CustomData = Data;
}
