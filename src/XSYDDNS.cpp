#include <XSYDDNS.h>

void EasyCrossPlatform::Network::Socket::DNSRequest::m_uv_resolved_cb(uv_getaddrinfo_t * resolver, int status, addrinfo * res)
{
	DNSRequest* MyClass = (DNSRequest*)resolver->data;
	if (status < 0) {
		MyClass->progressSucceed = false;
		MyClass->onProgress = false;
		uv_freeaddrinfo(res);
		return;
	}
	MyClass->progressSucceed = true;
	MyClass->valuebackArray.clear();
	addrinfo* myResAddr = res;
	
	while (myResAddr != NULL) {
		MyClass->valuebackArray.push_back(IpAddr(((myResAddr->ai_addr))));
		if (myResAddr != res->ai_next) {
			myResAddr = res->ai_next;
		}
		else {
			myResAddr = NULL;
		}
	}
	
	//MyClass->valuebackArray.NumResults = 1;
	//MyClass->valuebackArray.Ips.push_back(IpAddr((*(myResAddr->ai_addr))));
	MyClass->onProgress = false;
	uv_freeaddrinfo(res);
	return;
}

void EasyCrossPlatform::Network::Socket::DNSRequest::resetHint()
{
	this->m_RequestHints.ai_addr = NULL;
	this->m_RequestHints.ai_addrlen = 0;
	this->m_RequestHints.ai_canonname = NULL;
	this->m_RequestHints.ai_family = PF_UNSPEC;
	this->m_RequestHints.ai_flags = 0;
	this->m_RequestHints.ai_next = NULL;
	this->m_RequestHints.ai_protocol = 0;
	this->m_RequestHints.ai_socktype = 0;
}

EasyCrossPlatform::Network::Socket::DNSRequest::DNSRequest()
{
	this->Init();
}

EasyCrossPlatform::Network::Socket::DNSRequest::DNSRequest(const DNSRequest & oldRequest)
{
	this->Init();
}

std::vector<EasyCrossPlatform::Network::Socket::IpAddr> EasyCrossPlatform::Network::Socket::DNSRequest::getDomainAddr_IPv4v6(const std::string& Domain, unsigned short Port)
{
	std::vector<EasyCrossPlatform::Network::Socket::IpAddr> myResultv4 = this->getDomainAddr_IPv4Only(Domain, Port);
	std::vector<EasyCrossPlatform::Network::Socket::IpAddr> myResultv6 = this->getDomainAddr_IPv6Only(Domain, Port);
	std::vector<EasyCrossPlatform::Network::Socket::IpAddr> myResultv4v6;
	myResultv4v6.reserve(myResultv4.size() + myResultv4v6.size());
	if (!myResultv4.empty()) {
		for (auto i = myResultv4.begin(); i != myResultv4.end(); i++) {
			myResultv4v6.push_back((*i));
		}
	}
	if (!myResultv6.empty()) {
		for (auto i = myResultv6.begin(); i != myResultv6.end(); i++) {
			myResultv4v6.push_back((*i));
		}
	}
	return myResultv4v6;
}

std::vector<EasyCrossPlatform::Network::Socket::IpAddr> EasyCrossPlatform::Network::Socket::DNSRequest::getDomainAddr_IPv4Only(const std::string& Domain, unsigned short Port)
{
	this->resetHint();
	this->m_RequestHints.ai_family = PF_INET;
	this->onProgress = true;
	this->progressSucceed = false;
	std::string PortStr;
	std::vector<EasyCrossPlatform::Network::Socket::IpAddr> myResult;
	myResult.clear();
	PortStr = std::to_string(Port);
	int reqState = uv_getaddrinfo(&SocketParam::m_uv_loop, this->m_RequestHandle, DNSRequest::m_uv_resolved_cb, Domain.c_str(), PortStr.c_str(), &this->m_RequestHints);
	if (reqState < 0) {
		return myResult;
	}
	while (this->onProgress) {
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
	}
	if (this->progressSucceed) {
		return this->valuebackArray;
	}
	else {
		return myResult;
	}
}

std::vector<EasyCrossPlatform::Network::Socket::IpAddr> EasyCrossPlatform::Network::Socket::DNSRequest::getDomainAddr_IPv6Only(const std::string& Domain, unsigned short Port)
{
	this->resetHint();
	this->m_RequestHints.ai_family = PF_INET6;
	this->onProgress = true;
	this->progressSucceed = false;
	std::string PortStr;
	std::vector<EasyCrossPlatform::Network::Socket::IpAddr> myResult;
	myResult.clear();
	PortStr = std::to_string(Port);
	int reqState = uv_getaddrinfo(&SocketParam::m_uv_loop, this->m_RequestHandle, DNSRequest::m_uv_resolved_cb, Domain.c_str(), PortStr.c_str(), &this->m_RequestHints);
	if (reqState < 0) {
		return myResult;
	}
	while (this->onProgress) {
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
	}
	if (this->progressSucceed) {
		return this->valuebackArray;
	}
	else {
		return myResult;
	}
}

void EasyCrossPlatform::Network::Socket::DNSRequest::Init()
{
	if (this->Inited) {
		return;
	}
	this->Inited = true;
	this->onProgress = false;
	this->m_RequestHandle = new uv_getaddrinfo_t;
	this->m_RequestHandle->data = (void*) this;
	if (SocketParam::m_num_Client == 0U) {
		SocketParam::Start();
	}
	SocketParam::m_num_Client++;
}

void EasyCrossPlatform::Network::Socket::DNSRequest::Destroy()
{
	if (!this->Inited) {
		return;
	}
	this->Inited = false;
	delete this->m_RequestHandle;
	SocketParam::m_num_Client--;
	if (SocketParam::m_num_Client == 0U) {
		SocketParam::Stop();
	}
}

EasyCrossPlatform::Network::Socket::DNSRequest::~DNSRequest()
{
	this->Destroy();
}
