#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;

class MySocket {
public:
	static void ConnectCB(bool Succeeded, void* ClassPtr) {
		EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket* MyClass = (EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket*) ClassPtr;
		if (Succeeded) {
			std::cout << "Socket Connected" << std::endl;
			MyClass->SendMsg("GET / HTTP/1.1\r\nHOST:www.kvm.ink\r\nConnection: Keep-Alive\r\n\r\n");
		}
	}
	static void MsgCB(const std::vector<byte>& Msg, void* ClassPtr) {
		EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket* MyClass = (EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket*) ClassPtr;
		std::cout << EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg);
	}
	static void DisconnectCB(void* ClassPtr) {
		EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket* MyClass = (EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket*) ClassPtr;
		std::cout << "SocketDis" << std::endl;
	}
	static void ErrorCB(int errCode, const std::string& errDescription, void* ClassPtr) {
		EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket* MyClass = (EasyCrossPlatform::Network::Socket::TLSAsyncClientSocket*) ClassPtr;
		std::cout << "Error:" << errDescription << std::endl;
	}
};

int main(int argc, char** args) {
	std::string myTrustedCA = EasyCrossPlatform::File::FileIO::ReadFile("E:\\C++\\EasyCrossPlatform_VS\\CALIST.txt");
	TCPAsyncClientSocket myClient;
	SocketWorker myWorker;
	myClient.setWorker(&myWorker);
	myClient.ConnectCallBack = MySocket::ConnectCB;
	myClient.DisconnectCallBack = MySocket::DisconnectCB;
	myClient.MsgCallBack = MySocket::MsgCB;
	myClient.ErrorCallBack = MySocket::ErrorCB;
	myClient.Init();
	myClient.setRemoteIPAddr(IpAddr("104.27.184.99", 443, true));
	myClient.setSelfPort(700); //This line is optional, you can delete this line and the system will allocate a port for your socket. This Line throws a std::runtime_error when encounter an error allocating the port.
	system("pause");
	myClient.Connect();
	system("pause");
	myClient.Disconnect();
	myClient.Destroy();

}