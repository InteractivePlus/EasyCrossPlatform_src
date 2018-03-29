#include <XSYDHTTPMethodDec.h>

std::string std::to_string(const EasyCrossPlatform::Network::Request::RequestMethod myMethod)
{
	if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::GET) {
		return "GET";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::POST) {
		return "POST";
	}
	else { //myMethod == UNSPEC, else
		return "";
	}
}

std::wstring std::to_wstring(const EasyCrossPlatform::Network::Request::RequestMethod myMethod)
{
	if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::GET) {
		return L"GET";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::POST) {
		return L"POST";
	}
	else { //myMethod == UNSPEC, else
		return L"";
	}
}

EasyCrossPlatform::Network::Request::RequestMethod std::storm(const std::string & myMethod)
{
	std::string mMethod = EasyCrossPlatform::Parser::StringUtil::toUpper(myMethod);
	if (mMethod == "GET") {
		return EasyCrossPlatform::Network::Request::RequestMethod::GET;
	}
	else if (mMethod == "POST") {
		return EasyCrossPlatform::Network::Request::RequestMethod::POST;
	}
	else {
		return EasyCrossPlatform::Network::Request::RequestMethod::UNSPEC;
	}
}

EasyCrossPlatform::Network::Request::RequestMethod std::wstorm(const std::wstring & myMethod)
{
	std::wstring mMethod = EasyCrossPlatform::Parser::StringUtil::toUpper(myMethod);
	if (mMethod == L"GET") {
		return EasyCrossPlatform::Network::Request::RequestMethod::GET;
	}
	else if (mMethod == L"POST") {
		return EasyCrossPlatform::Network::Request::RequestMethod::POST;
	}
	else {
		return EasyCrossPlatform::Network::Request::RequestMethod::UNSPEC;
	}
}
