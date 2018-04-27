#include <EasyCrossPlatform.h>
#include <iostream>
//This is a full example example of full query HTTP document.
//As you can see, it follows the 302 Redirect by the root file and goes to /en/ directory
int main(int argc, char** args) {
	EasyCrossPlatform::Network::Request::WebPageDownload mDownload = EasyCrossPlatform::Network::Request::WebPageDownload();
	mDownload.PerformGetRequest("https://www.xsyds.cn/");
	std::cout << "XSYDS.CN" << std::endl;
	std::cout << "-----------------------------------------------------" << std::endl;
	std::cout << EasyCrossPlatform::Parser::StringUtil::fromBytes(mDownload.DownloadedData) << std::endl;
	system("pause");
	return 0;
}