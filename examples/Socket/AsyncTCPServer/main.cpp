#include "EasyCrossPlatform.h"
#include <iostream>
//Async TCP Server Test
class MyServer : public EasyCrossPlatform::Network::TCPAsyncSocket {
public:
	void ConnectedCallBack(EasyCrossPlatform::Network::TCPSocket &MySocket, std::mutex &ConnectionMutex) {
		std::cout << "Connected:" << MySocket.GetRemoteAddr() << "(" << MySocket.GetRemotePort() << ")" << std::endl;
	}
	void MsgCallBack(EasyCrossPlatform::Network::TCPSocket &MySocket, std::string Msg, std::mutex &MsgMutex) {
		std::cout << "Msg:" << MySocket.GetRemoteAddr() << "(" << MySocket.GetRemotePort() << ")[" << Msg << "]" << std::endl;
	}
	void DisconnectCallBack(EasyCrossPlatform::Network::TCPSocket &MySocket, std::mutex &ConnectionMutex) {
		std::cout << "Disconnected:" << MySocket.GetRemoteAddr() << "(" << MySocket.GetRemotePort() << ")" << std::endl;
	}
};
//AsyncServer Test
int main_AsyncServer(int argc, char** args){
	MyServer mServer;
	mServer.Listen(900);
	char Input[100];
	std::cout << "Press Any Key to Continue" << std::endl;
    std::cin >> Input;
    std::cout << "Bye!" << std::endl;
	mServer.StopListen();
	return 0;
}