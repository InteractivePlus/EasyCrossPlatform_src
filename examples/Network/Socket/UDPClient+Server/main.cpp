#include <EasyCrossPlatform.h>
#include <iostream>

class MySocket : public EasyCrossPlatform::Network::Socket::UDPAsyncSocket{
	public:
	//Inherit from Constructor, You can choose not to inherit constructor as long as you use Class.Init() and Class.Destroy() in your code.
	MySocket() : UDPAsyncSocket() {}
	MySocket(const EasyCrossPlatform::Network::Socket::IpAddr& RemoteIP) : UDPAsyncSocket(RemoteIP) {}
	~MySocket() { UDPAsyncSocket::~UDPAsyncSocket(); }
	//Inherit Finish

	void onMsg(const EasyCrossPlatform::Network::Socket::IpAddr& SourceIP, const std::string& data){
		std::cout << "SocketMsg:(" << ((EasyCrossPlatform::Network::Socket::IpAddr) SourceIP).getIPString() << "), " << data << std::endl;
	}
	void onError(int errCode, const std::string& errDescription) {
		std::cout << "Error:" << errDescription << std::endl;
	}
};
int main(int argc, char** args) {
	std::cout << "hi" << std::endl;
	
	MySocket mSocket;
	EasyCrossPlatform::Network::Socket::SocketWorker myWorker;
	mSocket.setWorker(myWorker);
	mSocket.Init();
	mSocket.Listen(EasyCrossPlatform::Network::Socket::IpAddr("0.0.0.0", 700, true));
	//UDP: If you want every computer in the LAN to recieve, type 255.255.255.255 for IP Address.
	system("pause");
	mSocket.sendMsg(EasyCrossPlatform::Network::Socket::IpAddr("127.0.0.1", 700, true),"Hi");
	system("pause");
	mSocket.StopListen();
	mSocket.Destroy();
	return 0;
}