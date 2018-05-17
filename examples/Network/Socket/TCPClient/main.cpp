#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;

class MySocket {
public:
	static void ConnectCB(bool Succeeded, StandardClientSocket* ClassPtr) {
		TCP::TCPAsyncClientSocket* MyClass = (TCP::TCPAsyncClientSocket*) ClassPtr;
		if (Succeeded) {
			std::cout << "Socket Connected" << std::endl;
		}
	}
	static void MsgCB(const std::vector<byte>& Msg, StandardClientSocket* ClassPtr) {
		TCP::TCPAsyncClientSocket* MyClass = (TCP::TCPAsyncClientSocket*) ClassPtr;
		std::cout << "Socket Msg:" << EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg) << std::endl;
	}
	static void DisconnectCB(StandardClientSocket* ClassPtr) {
		TCP::TCPAsyncClientSocket* MyClass = (TCP::TCPAsyncClientSocket*) ClassPtr;
		std::cout << "SocketDis" << std::endl;
	}
	static void ErrorCB(int errCode, const std::string& errDescription, StandardClientSocket* ClassPtr) {
		TCP::TCPAsyncClientSocket* MyClass = (TCP::TCPAsyncClientSocket*) ClassPtr;
		std::cout << "Error:" << errDescription << std::endl;
	}
};

int main(int argc, char** args) {
	IpAddr RemoteAddress = IpAddr("127.0.0.1", 700);
	SocketWorker myWorker;
	TCP::TCPAsyncClientSocket myClient = TCP::TCPAsyncClientSocket(RemoteAddress, &myWorker, 0U); //0U for self port means assign a random port.
	myClient.SetConnectCallBack(MySocket::ConnectCB);
	myClient.SetDisconnectCallBack(MySocket::DisconnectCB);
	myClient.SetMsgCallBack(MySocket::MsgCB);
	myClient.SetErrorCallBack(MySocket::ErrorCB);

	std::cout << "Press Any Key to Connect" << std::endl;
	std::cin.get();
	try {
		myClient.Connect();
	}
	catch (std::runtime_error e) {
		std::cout << "Connect Error: " << e.what() << std::endl;
	}
	std::cout << "Press Any Key to send \"233\"" << std::endl;
	std::cin.get();
	try {
		myClient.SendMsg("233");
	}
	catch (std::runtime_error e) {
		std::cout << "SendMsg Error: " << e.what() << std::endl;
	}
	std::cout << "Press Any Key to Disconnect" << std::endl;
	std::cin.get();
	myClient.Disconnect();
}