#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Parser;
int main(int argc, char** args) {
	HTTP::HTTPResponse mResponseCls;
	std::string ContentToParse = "HTTP/1.1 400 Bad Request\r\nDate: Sun, 18 Oct 2012 10:36:20 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nContent-Length: 5\r\nContent-Type: text/html; charset=iso-8859-1\r\nConnection: Closed\r\n\r\nData5";
	mResponseCls.fromResponseString(ContentToParse);
	std::cout << "Parse:" << std::endl;
	std::cout << ContentToParse << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "Code:" << std::to_string(mResponseCls.ResponseCode) << mResponseCls.ResponseDescription << std::endl;
	std::cout << "Major Version:" << std::to_string(mResponseCls.MajorVersion) << std::endl;
	std::cout << "Minor Version:" << std::to_string(mResponseCls.MinorVersion) << std::endl;
	std::cout << "Tranfer-Encodings:" << std::endl;
	for (auto i = mResponseCls.TransferEncoding.begin(); i != mResponseCls.TransferEncoding.end(); i++) {
		std::cout << "	Encoding: " << (*i) << std::endl;
	}
	std::cout << "Content:" << std::endl;
	std::cout << mResponseCls.OriginalData << std::endl;
	std::cout << "----------------" << std::endl;
	mResponseCls.cleanUp();
	system("pause");
}