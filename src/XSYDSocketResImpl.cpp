#include <XSYDSocketResImpl.h>

uv_loop_t EasyCrossPlatform::Network::Socket::SocketParam::m_uv_loop;
unsigned int EasyCrossPlatform::Network::Socket::SocketParam::m_num_Client = 0;
EasyCrossPlatform::Thread::SingleWork EasyCrossPlatform::Network::Socket::SocketParam::m_MTManager = EasyCrossPlatform::Thread::SingleWork(SocketParam::m_MultiThread_Job);

void EasyCrossPlatform::Network::Socket::SocketParam::m_MultiThread_Job(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	while ((*RunningSign)){
		uv_run(&SocketParam::m_uv_loop, UV_RUN_DEFAULT);
	}
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