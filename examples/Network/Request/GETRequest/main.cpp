#include "EasyCrossPlatform.h"
#include <iostream>
//Get Request to xsyds.cn
int main(int argc, char** args){
	EasyCrossPlatform::Network::Request::WebsiteRequestInfo MyWebInfo;
	EasyCrossPlatform::Network::Request::WebsiteRequest MyRequest;
	MyWebInfo = MyRequest.getURL("https://www.xsyds.cn",true);
	std::cout << MyWebInfo.Body << std::endl;
	return 0;
}