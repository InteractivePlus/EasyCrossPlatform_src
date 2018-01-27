#include <EasyCrossPlatform.h>
#include <iostream>

class MySocket : public EasyCrossPlatform::Network::Socket::UDPAsyncSocketv4{
	public:
	//Inherit from Constructor, You can choose not to inherit constructor as long as you use Class.Init() and Class.Destroy() in your code.
	MySocket() : UDPAsyncSocketv4() {}
	MySocket(const EasyCrossPlatform::Network::Socket::IpAddrV4& RemoteIP) : UDPAsyncSocketv4(RemoteIP) {}
	~MySocket() { UDPAsyncSocketv4::~UDPAsyncSocketv4(); }
	//Inherit Finish

	void onMsg(const EasyCrossPlatform::Network::Socket::IpAddrV4& SourceIP, const std::string& data){
		std::cout << "SocketMsg:(" << ((EasyCrossPlatform::Network::Socket::IpAddrV4) SourceIP).getIPString() << "), " << data << std::endl;
	}
	void onError(int errCode, const std::string& errDescription) {
		std::cout << "Error:" << errDescription << std::endl;
	}
};
int main(int argc, char** args) {
	std::cout << "hi" << std::endl;
	
	MySocket mSocket;
	mSocket.Init();
	mSocket.Listen(EasyCrossPlatform::Network::Socket::IpAddrV4("0.0.0.0", 700));
	system("pause");
	mSocket.sendMsg(EasyCrossPlatform::Network::Socket::IpAddrV4("127.0.0.1", 700),"Hi");
	system("pause");
	mSocket.StopListen();
	mSocket.Destroy();
	return 0;
}