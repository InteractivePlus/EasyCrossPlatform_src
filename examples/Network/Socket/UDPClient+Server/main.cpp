#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;

class MySocket {
public:
	static void onMsg(const std::vector<byte>& Data, const IpAddr& SourceIP, MailRecieverSocket* Reciever) {
		std::cout << "SocketMsg:(" << ((IpAddr)SourceIP).getIPString() << "), " << EasyCrossPlatform::Parser::StringUtil::fromBytes(Data) << std::endl;
	}
	static void onError(int errCode, const std::string& errDescription, MailRecieverSocket* Reciever) {
		std::cout << "Error:" << errDescription << std::endl;
	}
};
int main(int argc, char** args) {
	SocketWorker myWorker;
	IpAddr BindingAddr = IpAddr("::", 700U);
	IpAddr SendAddr = IpAddr("::1", 700U);

	//Setting Up
	UDP::UDPAsyncClientAndServerSocket mSocket = UDP::UDPAsyncClientAndServerSocket(BindingAddr, &myWorker);
	mSocket.SetServerErrCallBack(MySocket::onError);
	mSocket.SetServerMsgCallBack(MySocket::onMsg);

	std::cout << "Press any key to start listen" << std::endl;
	std::cin.get();
	mSocket.StartListen();

	std::cout << "Press any key to send \"Hi\" to yourself" << std::endl;
	std::cin.get();
	try {
		mSocket.SendMsg(SendAddr, "Hi");//OnlyUDP: If you want every computer in the LAN to recieve, type 255.255.255.255 for IP Address.
	}
	catch (std::runtime_error e) {
		std::cout << "Error happens while trying to send message:" << e.what() << std::endl;
	}

	std::cout << "Press any key to stop listening" << std::endl;
	std::cin.get();
	mSocket.StopListen();
	return 0;
}