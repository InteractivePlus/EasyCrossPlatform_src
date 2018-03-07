#ifndef __XSYDHTTPParserFile__
	#define __XSYDHTTPParserFile__
	#include <EasyCP_Common.h>
	#include <httpParser/http_parser.h>
	#include <XSYDStringUtil.h>
	namespace EasyCrossPlatform {
		namespace Parser {
			namespace HTTP {
				class URL {
					private:

					protected:
						
					public:
						std::string Protocol = ""; //Example: http
						std::string AccessCert = ""; //Example: Empty / admin:xsyd666
						std::string Host = ""; //Example: www.xsyds.cn
						unsigned short Port = 0U; //Example: 80U
						std::string Path = ""; //Example: "abc/def"
						std::string QueryString = ""; //Example: abc=1
						std::string FileName = ""; //Example=index.html
						std::string FragmentID = ""; //Example=div1
						std::string toURLString();
						void fromURLString(const std::string& urlString);
						void setAccessCert(const std::string& Username, const std::string& Password);
						std::pair<std::string, std::string> getAccessCert();
						void cleanUp();
				};
				class HTTPRequest {
					private:

					protected:
						void analyzeFirstLine(std::string& firstLine);
					public:
						unsigned short majorVersion = 0U; //Major version of http, can be 1U or 2U
						unsigned short minorVersion = 0U; //Minor version of http, can be 0U or 1U
						URL RequestURL; //host can be left empty due to http1.0
						void* customData = NULL;
						std::string method = "";
						std::string connection = "";
						std::string data = "";
						std::map<std::string, std::string> fields;
						void cleanUp();
						bool fromReqString(const std::string& Request);
						std::string toReqString();
				};
				class HTTPResponse {
					private:

					protected:

					public:
						unsigned short int statusCode = 0U;
						std::string statusDescription = "";
						unsigned short int majorVersion = 0U;
						unsigned short int minorVersion = 0U;
						std::string data = "";
						std::map<std::string, std::string> fields;
						void cleanUp();
						bool fromResponseString(const std::string& Request);
						std::string toResponseString();
				};
			}
		}
	}
#endif