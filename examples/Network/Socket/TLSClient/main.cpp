#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;

class MySocket {
public:
	static void ConnectCB(bool Succeeded, TLSAsyncClientSocket* ClassPtr) {
		TLSAsyncClientSocket* MyClass = ClassPtr;
		if (Succeeded) {
			std::cout << "Socket Connected" << std::endl;
			MyClass->SendMsg("GET / HTTP/1.1\r\nHOST:www.kvm.ink\r\nConnection: Keep-Alive\r\n\r\n");
		}
	}
	static void MsgCB(const std::vector<byte>& Msg, TLSAsyncClientSocket* ClassPtr) {
		TLSAsyncClientSocket* MyClass = ClassPtr;
		std::cout << EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg);
	}
	static void DisconnectCB(TLSAsyncClientSocket* ClassPtr) {
		TLSAsyncClientSocket* MyClass = ClassPtr;
		std::cout << "SocketDis" << std::endl;
	}
	static void ErrorCB(int errCode, const std::string& errDescription, TLSAsyncClientSocket* ClassPtr) {
		TLSAsyncClientSocket* MyClass = ClassPtr;
		std::cout << "Error:" << errDescription << std::endl;
	}
};

int main(int argc, char** args) {
	std::string myTrustedCA = EasyCrossPlatform::File::FileIO::ReadFile(EasyCrossPlatform::Runtime::Path::APPPath() + "CALIST.txt");
	TLSAsyncClientSocket myClient;
	SocketWorker myWorker;
	myClient.setWorker(&myWorker);
	myClient.ConnectCallBack = MySocket::ConnectCB;
	myClient.DisconnectCallBack = MySocket::DisconnectCB;
	myClient.MsgCallBack = MySocket::MsgCB;
	myClient.ErrorCallBack = MySocket::ErrorCB;
	myClient.VerifyServerCert = true;
	myClient.Init();
	myClient.setRemoteIPAddr(IpAddr("104.27.184.99", 443, true));
	myClient.setSelfPort(700); //This Line is Optional, you can delete it so that the Port will be assigned by the system. It throws out Runtime Error when an error is occured when binding
	myClient.setTrustedCAChain(myTrustedCA);
	myClient.serverHostName = "www.kvm.ink";
	system("pause");
	myClient.Connect();
	system("pause");
	myClient.Disconnect();
	myClient.Destroy();

}