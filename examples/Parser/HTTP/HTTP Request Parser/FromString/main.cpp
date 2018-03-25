#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Parser;
int main(int argc, char** args) {
	HTTP::HTTPRequest mRequestCls;
	std::string ContentToParse = "GET / HTTP/1.1\r\nHost:www.xsyds.cn\r\nAccept-Encoding: br;q=1.0, gzip;q=0.8, deflate;q=0.8\r\nContent-Length: 0\r\n\r\n";
	mRequestCls.fromReqString(ContentToParse);
	std::cout << "Parse:" << std::endl;
	std::cout << ContentToParse << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "Method:" << mRequestCls.Method << std::endl;
	std::cout << "URL:" << mRequestCls.RequestedURL.toURLString() << std::endl;
	std::cout << "Major Version:" << std::to_string(mRequestCls.MajorVersion) << std::endl;
	std::cout << "Minor Version:" << std::to_string(mRequestCls.MinorVersion) << std::endl;
	std::cout << "Accept-Encodings:" << std::endl;
	for (auto i = mRequestCls.AcceptEncoding.begin(); i != mRequestCls.AcceptEncoding.end(); i++) {
		std::cout << "	Encoding: " << (*i).first << ",Weight: " << std::to_string((*i).second) << std::endl;
	}
	std::cout << "----------------" << std::endl;
	mRequestCls.cleanUp();
	system("pause");
}