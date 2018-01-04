#ifndef __XSYDREQUESTFILE__
	#define __XSYDREQUESTFILE__
	#include <EasyCP_Common.h>
	#include <curl/curl.h>
	#include <mutex>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Request {
				struct WebsiteRequestInfo {
					CURLcode ErrNo;
					bool Succeed;
					long ServerResponseCode;
					std::string Header;
					std::string Body;
				};
				class WebsiteRequest {
				private:
					CURL *m_myCurl = NULL;
					std::mutex m_myThreadMutex;
					static size_t writeFunction(void *ptr, size_t dataTypesize, size_t dataNum, void* stream);
					static std::string convertPostMapToString(const std::map<std::string, std::string> &myMap);
				public:
					WebsiteRequest();
					~WebsiteRequest();
					WebsiteRequestInfo getURL(const std::string &URL, bool SSLCertCheck = true);
					WebsiteRequestInfo postURL(const std::string &URL, const std::map<std::string, std::string> &PostValue, bool SSLCertCheck = true);
				};
			}
		}
	}
#endif