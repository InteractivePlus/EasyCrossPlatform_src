#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;

int main(int argc, char** args) {
	std::cout << "hi" << std::endl;
	DNSRequest myRequest;
	myRequest.Init();
	DNSResult myResult = myRequest.getDomainAddr_IPv4v6("www.xsyds.cn",80);
	std::cout << "Request " << (myResult.NumResults != 0 ? "succeeded" : "failed") << std::endl;
	if (myResult.NumResults > 0) {
		for (auto iter = myResult.Ips.begin(); iter != myResult.Ips.end(); iter++) {
			std::cout << "IpAddress: " << (*iter).getIPString() << std::endl;
		}
	}
	myRequest.Destroy();

	std::cin.get();
	
	return 0;
}