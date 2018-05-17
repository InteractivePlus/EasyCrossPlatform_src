#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;

class MySocket {
public:
	static void ConnectCB(bool Succeeded, StandardClientSocket* ClassPtr) {
		TLS::TLSAsyncClientSocket* MyClass = (TLS::TLSAsyncClientSocket*) ClassPtr;
		if (Succeeded) {
			std::cout << "Socket Connected" << std::endl;
			MyClass->SendMsg("GET / HTTP/1.1\r\nHOST:www.kvm.ink\r\nConnection: Keep-Alive\r\n\r\n");
		}
	}
	static void MsgCB(const std::vector<byte>& Msg, StandardClientSocket* ClassPtr) {
		TLS::TLSAsyncClientSocket* MyClass = (TLS::TLSAsyncClientSocket*) ClassPtr;
		std::cout << EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg);
	}
	static void DisconnectCB(StandardClientSocket* ClassPtr) {
		TLS::TLSAsyncClientSocket* MyClass = (TLS::TLSAsyncClientSocket*) ClassPtr;
		std::cout << "SocketDis" << std::endl;
	}
	static void ErrorCB(int errCode, const std::string& errDescription, StandardClientSocket* ClassPtr) {
		TLS::TLSAsyncClientSocket* MyClass = (TLS::TLSAsyncClientSocket*) ClassPtr;
		std::cout << "Error:" << errDescription << std::endl;
	}
};

int main(int argc, char** args) {
	std::string myTrustedCA = EasyCrossPlatform::Network::Request::TrustedCA; //We prepared the Mozilla CA trust file for you, no need to read!
	IpAddr RemoteIP = IpAddr("104.27.184.99", 443);
	SocketWorker myWorker;

	TLS::TLSAsyncClientSocket myClient = TLS::TLSAsyncClientSocket(&myWorker, RemoteIP, true, "www.kvm.ink"); //SelfPort is set to 0U if you want it to generate a random port for you.
	myClient.SetConnectCallBack(MySocket::ConnectCB);
	myClient.SetDisconnectCallBack(MySocket::DisconnectCB);
	myClient.SetMsgCallBack(MySocket::MsgCB);
	myClient.SetErrorCallBack(MySocket::ErrorCB);
	myClient.setTrustedCAChain(myTrustedCA);
	std::cout << "Press any key to connect" << std::endl;
	std::cin.get();
	try {
		myClient.Connect();
	}
	catch (std::runtime_error e) {
		std::cout << "Connect Error: " << e.what() << std::endl;
	}
	std::cout << "Press any key to disconnect" << std::endl;
	std::cin.get();
	myClient.Disconnect();
	return 0;
}