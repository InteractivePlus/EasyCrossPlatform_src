#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Parser;

int main(int argc, char** args) {
	HTTP::HTTPRequest myReq;
	std::string StrToParse =
		"GET / HTTP/1.1\nHOST:www.xsyds.cn\n\n<!doctype HTML><html><head><title>Test</title></head></html>";
		//"XSYDS.CN";
	try {
		bool myRes = myReq.fromReqString(StrToParse); //if returns false, means it needs more data to feed.
	}
	catch (std::invalid_argument e) {
		//Str To Parse is not a proper HTTP Request
	}
	std::cout << "Original Msg:" << std::endl;
	std::cout << StrToParse << std::endl;
	std::cout << "-------------" << std::endl;
	std::cout << myReq.toReqString() << std::endl;
	system("pause");
}