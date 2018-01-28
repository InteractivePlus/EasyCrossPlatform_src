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
	MyClass->valuebackArray.NumResults = 0;
	MyClass->valuebackArray.Ips.clear();
	addrinfo* myResAddr = res;
	
	while (myResAddr != NULL) {
		MyClass->valuebackArray.NumResults++;
		MyClass->valuebackArray.Ips.push_back(IpAddr((*(myResAddr->ai_addr))));
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

EasyCrossPlatform::Network::Socket::DNSResult EasyCrossPlatform::Network::Socket::DNSRequest::getDomainAddr_IPv4v6(const std::string& Domain, unsigned short Port)
{
	DNSResult myResultv4 = this->getDomainAddr_IPv4Only(Domain, Port);
	DNSResult myResultv6 = this->getDomainAddr_IPv6Only(Domain, Port);
	DNSResult myResultv4v6;
	myResultv4v6.NumResults = myResultv4.NumResults + myResultv6.NumResults;
	myResultv4v6.Ips.reserve(myResultv4v6.NumResults);
	if (!myResultv4.Ips.empty()) {
		for (auto i = myResultv4.Ips.begin(); i != myResultv4.Ips.end(); i++) {
			myResultv4v6.Ips.push_back((*i));
		}
	}
	if (!myResultv6.Ips.empty()) {
		for (auto i = myResultv6.Ips.begin(); i != myResultv6.Ips.end(); i++) {
			myResultv4v6.Ips.push_back((*i));
		}
	}
	return myResultv4v6;
}

EasyCrossPlatform::Network::Socket::DNSResult EasyCrossPlatform::Network::Socket::DNSRequest::getDomainAddr_IPv4Only(const std::string& Domain, unsigned short Port)
{
	this->resetHint();
	this->m_RequestHints.ai_family = PF_INET;
	this->onProgress = true;
	this->progressSucceed = false;
	char PortStr[6] = "";
	DNSResult myResult;
	myResult.NumResults = 0;
	#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
		itoa(static_cast<int>(Port), PortStr, 10);
	#elif defined(EASYCROSSPLATFORM_PLATFORM_WINDOWS)
		_itoa_s(static_cast<int>(Port), PortStr, 6, 10);
	#endif
	int reqState = uv_getaddrinfo(&SocketParam::m_uv_loop, this->m_RequestHandle, DNSRequest::m_uv_resolved_cb, Domain.c_str(), PortStr, &this->m_RequestHints);
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

EasyCrossPlatform::Network::Socket::DNSResult EasyCrossPlatform::Network::Socket::DNSRequest::getDomainAddr_IPv6Only(const std::string& Domain, unsigned short Port)
{
	this->resetHint();
	this->m_RequestHints.ai_family = PF_INET6;
	this->onProgress = true;
	this->progressSucceed = false;
	char PortStr[6] = "";
	DNSResult myResult;
	myResult.NumResults = 0;
	#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
		itoa(static_cast<int>(Port), PortStr, 10);
	#elif defined(EASYCROSSPLATFORM_PLATFORM_WINDOWS)
		_itoa_s(static_cast<int>(Port), PortStr, 6, 10);
	#endif
	int reqState = uv_getaddrinfo(&SocketParam::m_uv_loop, this->m_RequestHandle, DNSRequest::m_uv_resolved_cb, Domain.c_str(), PortStr, &this->m_RequestHints);
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
	if (SocketParam::m_num_Client == 0) {
		uv_loop_init(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StartJob();
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
	if (SocketParam::m_num_Client == 0) {
		uv_stop(&SocketParam::m_uv_loop);
		SocketParam::m_MTManager.StopJob();
		uv_loop_close(&SocketParam::m_uv_loop);
	}
}

EasyCrossPlatform::Network::Socket::DNSRequest::~DNSRequest()
{
	this->Destroy();
}
