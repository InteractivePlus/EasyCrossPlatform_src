#ifndef __XSYDHTTPDOWNLOADFILE__
	#define __XSYDHTTPDOWNLOADFILE__
	#include <EasyCP_Common.h>
	#include <XSYDRequest.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Request {
				const constexpr unsigned int MaxRedirectTime = 5U;
				const constexpr char* AcceptLanguageDefaultString = "zh-CN;q=1.0, zh;q=0.9, en-US;q=0.8, en;q=0.5, *;q=0.2";
				class WebPageDownload {
				private:

				protected:
					void PerformRequest(const RequestMethod mRequestMethod, const std::string& URL, const std::string& PostData = 0, const unsigned int RecursionTime = 0);
					
				public:
					std::vector<byte> DownloadedData;
					EasyCrossPlatform::Parser::HTTP::HTTPResponse mResponseContent = EasyCrossPlatform::Parser::HTTP::HTTPResponse();
					bool DownloadSucceed = false;
					void cleanUp();
					void PerformGetRequest(const std::string& URL);
					void PerformPostRequest(const std::string& URL, const std::string& PostData);
					std::string AcceptLanguageString = AcceptLanguageDefaultString;
				};
			}
		}
	}
#endif