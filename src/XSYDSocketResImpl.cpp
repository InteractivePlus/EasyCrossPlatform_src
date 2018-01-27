#include <XSYDSocketResImpl.h>
#include "..\include\XSYDSocketResImpl.h"
//IpAddrV4 Class
EasyCrossPlatform::Network::Socket::IpAddrV4::IpAddrV4() : IpAddrV4("0.0.0.0", 0) {

}
EasyCrossPlatform::Network::Socket::IpAddrV4::IpAddrV4(const std::string& IpAddress, const unsigned short Port) : IpAddrV4(IpAddress.c_str(), Port) {

}
EasyCrossPlatform::Network::Socket::IpAddrV4::IpAddrV4(const char * IpAddress, const unsigned short Port) {
	uv_ip4_addr(IpAddress, static_cast<int>(Port), &(this->m_Addr));
}
bool EasyCrossPlatform::Network::Socket::IpAddrV4::setIPAddress(const std::string& IpAddress, const unsigned short Port) {
	return this->setIPAddress(IpAddress.c_str(), Port);
}
bool EasyCrossPlatform::Network::Socket::IpAddrV4::setIPAddress(const char * IpAddress, const unsigned short Port) {
	int setState = uv_ip4_addr(IpAddress, static_cast<int>(Port), &(this->m_Addr));
	if (setState < 0) {
		return false;
	}
	else {
		return true;
	}
}
bool EasyCrossPlatform::Network::Socket::IpAddrV4::setIPAddress(const sockaddr_in& newAddr) {
	this->m_Addr = newAddr;
	return true;
}
std::string EasyCrossPlatform::Network::Socket::IpAddrV4::getIPString() {
	char myNewAddr[13];
	uv_ip4_name(&(this->m_Addr), (char*)myNewAddr, 13);
	return std::string(myNewAddr);
}
unsigned short EasyCrossPlatform::Network::Socket::IpAddrV4::getPort() {
	return ntohs(this->m_Addr.sin_port);
}
sockaddr_in EasyCrossPlatform::Network::Socket::IpAddrV4::getIPAddress()
{
	return this->m_Addr;
}
//IpAddrV4 Class Ends

//IpAddrV6 Class
EasyCrossPlatform::Network::Socket::IpAddrV6::IpAddrV6() : IpAddrV6("::", 0) {

}
EasyCrossPlatform::Network::Socket::IpAddrV6::IpAddrV6(const std::string& IpAddress, const unsigned short Port) : IpAddrV6(IpAddress.c_str(), Port) {

}
EasyCrossPlatform::Network::Socket::IpAddrV6::IpAddrV6(const char * IpAddress, const unsigned short Port) {
	uv_ip6_addr(IpAddress, static_cast<int>(Port), &(this->m_Addr));
}
bool EasyCrossPlatform::Network::Socket::IpAddrV6::setIPAddress(const std::string& IpAddress, const unsigned short Port) {
	return this->setIPAddress(IpAddress.c_str(), Port);
}
bool EasyCrossPlatform::Network::Socket::IpAddrV6::setIPAddress(const char * IpAddress, const unsigned short Port) {
	int setState = uv_ip6_addr(IpAddress, static_cast<int>(Port), &(this->m_Addr));
	if (setState < 0) {
		return false;
	}
	else {
		return true;
	}
}
bool EasyCrossPlatform::Network::Socket::IpAddrV6::setIPAddress(const sockaddr_in6& newAddr) {
	this->m_Addr = newAddr;
	return true;
}
std::string EasyCrossPlatform::Network::Socket::IpAddrV6::getIPString() {
	char myNewAddr[40];
	uv_ip6_name(&(this->m_Addr), (char*)myNewAddr, 40);
	return std::string(myNewAddr);
}
unsigned short EasyCrossPlatform::Network::Socket::IpAddrV6::getPort() {
	return ntohs(this->m_Addr.sin6_port);
}
sockaddr_in6 EasyCrossPlatform::Network::Socket::IpAddrV6::getIPAddress()
{
	return this->m_Addr;
}
//IpAddrV6 Class Ends

uv_loop_t EasyCrossPlatform::Network::Socket::SocketParam::m_uv_loop;
unsigned int EasyCrossPlatform::Network::Socket::SocketParam::m_num_Client = 0;
std::mutex EasyCrossPlatform::Network::Socket::SocketParam::Lock;
EasyCrossPlatform::Thread::SingleWork EasyCrossPlatform::Network::Socket::SocketParam::m_MTManager = EasyCrossPlatform::Thread::SingleWork(SocketParam::m_MultiThread_Job);

void EasyCrossPlatform::Network::Socket::SocketParam::m_MultiThread_Job(std::thread::id ThreadID, void * Parameters, bool * RunningSign, std::mutex * Mutex)
{
	while ((*RunningSign)){
		Lock.lock();
			uv_run(&SocketParam::m_uv_loop, UV_RUN_ONCE);
		Lock.unlock();
	}
	return;
}

EasyCrossPlatform::Network::Socket::IpAddrV6::IpAddrV6()
{
}
