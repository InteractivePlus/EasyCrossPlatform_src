#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Parser;
int main(int argc, char** args) {
	HTTP::URL myURL;
	myURL.fromURLString("https://www.xsyds.cn/en/index.html?query=true#div1");
	std::cout << "Protocol:" << myURL.Protocol << std::endl;
	std::cout << "Cert:" << myURL.AccessCert << std::endl;
	std::cout << "Host:" << myURL.Host << std::endl;
	std::cout << "Port:" << std::to_string(myURL.Port) << std::endl;
	std::cout << "Path:" << myURL.Path << std::endl;
	std::cout << "File:" << myURL.FileName << std::endl;
	std::cout << "QueryStr:" << myURL.QueryString << std::endl;
	std::cout << "FragmentID:" << myURL.FragmentID << std::endl;
	std::cout << "URL:" << myURL.toURLString() << std::endl;
	system("pause");
}