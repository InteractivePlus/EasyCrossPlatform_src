#ifndef __XSYDHTTPMethodDecFILE__
	#define __XSYDHTTPMethodDecFILE__
	#include <EasyCP_Common.h>
	#include <XSYDStringUtil.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Request {
				enum RequestMethod {
					GET,
					POST,
					HEAD,
					PUT,
					DEL,
					CONNECT,
					OPTIONS,
					TRACE,
					UNSPEC,
				};
			}
		}
	}
	namespace std {
		string to_string(const EasyCrossPlatform::Network::Request::RequestMethod myMethod);
		wstring to_wstring(const EasyCrossPlatform::Network::Request::RequestMethod myMethod);
		EasyCrossPlatform::Network::Request::RequestMethod storm(const std::string& myMethod);
		EasyCrossPlatform::Network::Request::RequestMethod wstorm(const std::wstring& myMethod);
	}
#endif