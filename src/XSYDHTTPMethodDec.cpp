#include <XSYDHTTPMethodDec.h>

std::string std::to_string(const EasyCrossPlatform::Network::Request::RequestMethod myMethod)
{
	if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::GET) {
		return "GET";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::POST) {
		return "POST";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::HEAD) {
		return "HEAD";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::PUT) {
		return "PUT";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::DEL) {
		return "DELETE";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::CONNECT) {
		return "CONNECT";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::OPTIONS) {
		return "OPTIONS";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::TRACE) {
		return "TRACE";
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
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::HEAD) {
		return L"HEAD";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::PUT) {
		return L"PUT";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::DEL) {
		return L"DELETE";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::CONNECT) {
		return L"CONNECT";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::OPTIONS) {
		return L"OPTIONS";
	}
	else if (myMethod == EasyCrossPlatform::Network::Request::RequestMethod::TRACE) {
		return L"TRACE";
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
	else if (mMethod == "HEAD") {
		return EasyCrossPlatform::Network::Request::RequestMethod::HEAD;
	}
	else if (mMethod == "PUT") {
		return EasyCrossPlatform::Network::Request::RequestMethod::PUT;
	}
	else if (mMethod == "DELETE") {
		return EasyCrossPlatform::Network::Request::RequestMethod::DEL;
	}
	else if (mMethod == "CONNECT") {
		return EasyCrossPlatform::Network::Request::RequestMethod::CONNECT;
	}
	else if (mMethod == "OPTIONS") {
		return EasyCrossPlatform::Network::Request::RequestMethod::OPTIONS;
	}
	else if (mMethod == "TRACE") {
		return EasyCrossPlatform::Network::Request::RequestMethod::TRACE;
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
	else if (mMethod == L"HEAD") {
		return EasyCrossPlatform::Network::Request::RequestMethod::HEAD;
	}
	else if (mMethod == L"PUT") {
		return EasyCrossPlatform::Network::Request::RequestMethod::PUT;
	}
	else if (mMethod == L"DELETE") {
		return EasyCrossPlatform::Network::Request::RequestMethod::DEL;
	}
	else if (mMethod == L"CONNECT") {
		return EasyCrossPlatform::Network::Request::RequestMethod::CONNECT;
	}
	else if (mMethod == L"OPTIONS") {
		return EasyCrossPlatform::Network::Request::RequestMethod::OPTIONS;
	}
	else if (mMethod == L"TRACE") {
		return EasyCrossPlatform::Network::Request::RequestMethod::TRACE;
	}
	else {
		return EasyCrossPlatform::Network::Request::RequestMethod::UNSPEC;
	}
}
