#include "EasyCrossPlatform.h"
#include <iostream>
//Blocked TCP Socket Test
int main(int argc, char** args) {
	EasyCrossPlatform::Network::TCPSocket MySock;
	MySock.Create();
	MySock.Bind(700);
	MySock.Listen();

	EasyCrossPlatform::Network::TCPSocket* ClientSock;
	char myBuffer[200] = "";
	int msgState;
	while (true) {
		msgState = -1;
		ClientSock = MySock.Accept();
		std::string MyStr = ClientSock->GetRemoteAddr();
		std::cout << "Connected:" << MyStr << std::endl;
		bool Connected = ClientSock->isConnected();
		while (msgState < 0) { //-1表示没有消息
			msgState = ClientSock->Receive(myBuffer, 200);
		}
		if (msgState == 0) {
			std::cout << "Client Connection Shutdown" << std::endl;
		}
		std::cout << myBuffer << std::endl;
		std::cout << strlen(myBuffer) << std::endl;
		ClientSock->Close();
		delete ClientSock;
	}
}