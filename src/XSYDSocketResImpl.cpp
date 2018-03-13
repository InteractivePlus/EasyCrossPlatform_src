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
	while ((*RunningSign)){
		if (uv_loop_alive(&SocketParam::m_uv_loop) > 0) {
			uv_run(&SocketParam::m_uv_loop, UV_RUN_DEFAULT);
		}
		else {
			std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
		}
	}
	uv_loop_close(&SocketParam::m_uv_loop);
	return;
}


EasyCrossPlatform::Network::Socket::IpAddr::IpAddr()
{
	this->m_isIpV4 = true;
}

EasyCrossPlatform::Network::Socket::IpAddr::IpAddr(const std::string & IpAddress, const unsigned short Port, bool AddrIpV4) : IpAddr(IpAddress.c_str(),Port,AddrIpV4)
{
	
}

EasyCrossPlatform::Network::Socket::IpAddr::IpAddr(const char * IpAddress, const unsigned short Port, bool AddrIpV4)
{
	this->setIPAddress(IpAddress, Port, AddrIpV4);
}

EasyCrossPlatform::Network::Socket::IpAddr::IpAddr(const sockaddr & newAddr)
{
	this->setIPAddress(newAddr);
}

EasyCrossPlatform::Network::Socket::IpAddr::IpAddr(const IpAddr & oldAddr)
{
	this->m_isIpV4 = oldAddr.m_isIpV4;
	this->m_Addr = oldAddr.m_Addr;
}

bool EasyCrossPlatform::Network::Socket::IpAddr::setIPAddress(const std::string & IpAddress, const unsigned short Port, bool AddrIpV4)
{
	return setIPAddress(IpAddress.c_str(), Port, AddrIpV4);
}

bool EasyCrossPlatform::Network::Socket::IpAddr::setIPAddress(const char * IpAddress, const unsigned short Port, bool AddrIpV4)
{
	int setState = 0;
	if (AddrIpV4) {
		setState = uv_ip4_addr(IpAddress, static_cast<int>(Port), (sockaddr_in*)&this->m_Addr);
	}
	else {
		setState = uv_ip6_addr(IpAddress, static_cast<int>(Port), (sockaddr_in6*)&this->m_Addr);
	}
	if (setState < 0) {
		return false;
	}
	else {
		return true;
	}
}

bool EasyCrossPlatform::Network::Socket::IpAddr::setIPAddress(const sockaddr & newAddr){
	if (newAddr.sa_family == AF_INET) {
		this->m_isIpV4 = true;
	}
	else if(newAddr.sa_family==AF_INET6) {
		this->m_isIpV4 = false;
	}
	else {
		return false;
	}
	this->m_Addr = newAddr;
	return true;
}

std::string EasyCrossPlatform::Network::Socket::IpAddr::getIPString()
{
	if (this->m_isIpV4) {
		char myNewAddr[16] = "";
		memset(myNewAddr, 0, 16);
		int State = uv_ip4_name((sockaddr_in*)&(this->m_Addr), (char*)myNewAddr, 16);
		return std::string(myNewAddr);
	}
	else {
		char myNewAddr[40] = "";
		memset(myNewAddr, 0, 40);
		int State = uv_ip6_name((sockaddr_in6*) &(this->m_Addr), (char*)myNewAddr, 40);
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
		sockaddr_in* mySocketAddr = (sockaddr_in*) &this->m_Addr;
		return ntohs(mySocketAddr->sin_port);
	}
	else {
		sockaddr_in6* mySocketAddr = (sockaddr_in6*)&this->m_Addr;
		return ntohs(mySocketAddr->sin6_port);
	}
}

sockaddr EasyCrossPlatform::Network::Socket::IpAddr::getIPAddress()
{
	return this->m_Addr;
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

void EasyCrossPlatform::Network::Socket::SocketWorker::m_MultiThread_Job(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	SocketWorker* myWorker = (SocketWorker*)Parameters;
	while ((*RunningSign)) {
		if (uv_loop_alive(myWorker->m_uv_loop.get()) > 0) {
			uv_run(myWorker->m_uv_loop.get(), UV_RUN_DEFAULT);
		}
		else {
			std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
		}
	}
	uv_loop_close(myWorker->m_uv_loop.get());
	return;
}

EasyCrossPlatform::Network::Socket::SocketWorker::~SocketWorker()
{
	
}
