#include <EasyCrossPlatform.h>
#include <iostream>

class MySocket {
public:
	static void ConnectCB(bool Succeeded, void* ClassPtr) {
		EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket* MyClass = (EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*) ClassPtr;
		std::cout << "Socket Connected" << std::endl;
	}
	static void MsgCB(const std::string& Msg, void* ClassPtr) {
		EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket* MyClass = (EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*) ClassPtr;
		std::cout << "SocketMsg:" << Msg << std::endl;
	}
	static void DisconnectCB(void* ClassPtr) {
		EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket* MyClass = (EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*) ClassPtr;
		std::cout << "SocketDis" << std::endl;
	}
	static void ErrorCB(int errCode, const std::string& errDescription, void* ClassPtr) {
		EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket* MyClass = (EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket*) ClassPtr;
		std::cout << "Error:" << errDescription << std::endl;
	}
};
int main(int argc, char** args) {
	std::cout << "hi" << std::endl;

	EasyCrossPlatform::Network::Socket::TCPAsyncClientSocket mSocket;
	EasyCrossPlatform::Network::Socket::SocketWorker mSocketWorker;
	mSocket.setWorker(mSocketWorker);
	mSocket.Init();
	//Set CallBack
	mSocket.ConnectCallBack = MySocket::ConnectCB;
	mSocket.MsgCallBack = MySocket::MsgCB;
	mSocket.DisconnectCallBack = MySocket::DisconnectCB;
	mSocket.ErrorCallBack = MySocket::ErrorCB;
	//Set CallBack finish.
	mSocket.setRemoteIPAddr(EasyCrossPlatform::Network::Socket::IpAddr("127.0.0.1", 700, true));
	mSocket.Connect();
	system("pause");
	mSocket.SendMsg("2333");
	system("pause");
	mSocket.Disconnect();
	mSocket.Destroy();
	return 0;
}