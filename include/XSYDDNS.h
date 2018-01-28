#ifndef __EasyCrossPlatform_DNSQueryFile__
	#define __EasyCrossPlatform_DNSQueryFile__
	#include <EasyCP_Common.h>
	#include <XSYDSocketResImpl.h>
	#include <libuv/uv.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Socket {
				struct DNSResult {
					unsigned int NumResults;
					std::vector<IpAddr> Ips;
				};
				class DNSRequest {
					private:

					protected:
						uv_getaddrinfo_t* m_RequestHandle;
						bool Inited;
						addrinfo m_RequestHints;
						bool onProgress;
						DNSResult valuebackArray;
						bool progressSucceed;
						static void m_uv_resolved_cb(uv_getaddrinfo_t* resolver, int status, struct addrinfo *res);
						void resetHint();
					public:
						DNSRequest();
						DNSRequest(const DNSRequest& oldRequest);

						DNSResult getDomainAddr_IPv4v6(const std::string& Domain, unsigned short Port);
						DNSResult getDomainAddr_IPv4Only(const std::string& Domain, unsigned short Port);
						DNSResult getDomainAddr_IPv6Only(const std::string& Domain, unsigned short Port);
						void Init();
						void Destroy();

						~DNSRequest();
				};
			}
		}
	}
#endif