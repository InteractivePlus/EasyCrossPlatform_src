#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Parser;
int main(int argc, char** args) {
	HTTP::URL myURL;
	myURL.fromURLString("https://www.xsyds.cn/en/index.html?query=true#div1");
	std::cout << "URL:" << myURL.toURLString() << std::endl;
	system("pause");
}