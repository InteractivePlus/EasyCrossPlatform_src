#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;

int main(int argc, char** args) {
	std::cout << "hi" << std::endl;
	std::vector<EasyCrossPlatform::Network::Socket::IpAddr> myRequest;
	myRequest.Init();
	std::vector<EasyCrossPlatform::Network::Socket::IpAddr> myResult = myRequest.getDomainAddr_IPv4v6("www.xsyds.cn",80);
	std::cout << "Request " << (!myResult.empty() ? "succeeded" : "failed") << std::endl;
	if (!myResult.empty()) {
		for (auto iter = myResult.begin(); iter != myResult.end(); iter++) {
			std::cout << "IpAddress: " << (*iter).getIPString() << std::endl;
		}
	}
	myRequest.Destroy();

	std::cin.get();
	
	return 0;
}