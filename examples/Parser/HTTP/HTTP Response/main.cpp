#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Parser;

int main(int argc, char** args) {
	HTTP::HTTPResponse myResponse;
	std::string StrToParse =
		"HTTP/1.0 200 OK\r\nDate: Fri, 31 Dec 1999 23:59:59 GMT\r\nContent-Type: text/html\r\nContent-Length: 4\r\n\r\nData";
	try {
		bool myRes = myResponse.fromResponseString(StrToParse); //if returns false, means it needs more data to feed.
	}
	catch (std::invalid_argument e) {
		//Str To Parse is not a proper HTTP Request
	}
	std::cout << "Original Msg:" << std::endl;
	std::cout << StrToParse << std::endl;
	std::cout << "-------------" << std::endl;
	std::cout << myResponse.toResponseString() << std::endl;
	system("pause");
}