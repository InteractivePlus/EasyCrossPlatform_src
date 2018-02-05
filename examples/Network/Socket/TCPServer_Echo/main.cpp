#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;
std::vector<TCPAsyncClientSocket*> myClients;
class MyServerFunction{
	public:
		static void onServerNewConn(void* newClientSocket, void* ServerClassPtr) {
			TCPAsyncClientSocket* MyClientSocket = (TCPAsyncClientSocket*) newClientSocket;
			TCPAsyncServerSocket* MyServer = (TCPAsyncServerSocket*)ServerClassPtr;
			myClients.push_back(MyClientSocket);
		}
		static void onServerError(int errCode, const std::string& errInfo, void* ServerClassPtr) {
			TCPAsyncServerSocket* MyServer = (TCPAsyncServerSocket*)ServerClassPtr;
			//std::cout << "SrvError:" << errInfo << std::endl;
		}
		static void onClientConnect(bool Succeed, void* ClientPtr){
			TCPAsyncClientSocket* MyClient = (TCPAsyncClientSocket*)ClientPtr;
			
			//std::cout << "ClientConnect:" << MyClient->getRemoteAddr().getIPString() << ":" << MyClient->getRemoteAddr().getPort() << std::endl;
		}
		static void onClientDisconnect(void* ClientPtr) {
			TCPAsyncClientSocket* MyClient = (TCPAsyncClientSocket*)ClientPtr;
			//std::cout << "ClientDisconnect:" << std::endl;
			MyClient->Destroy();
			for (auto i = myClients.begin(); i != myClients.end(); i++) {
				if ((*i) == MyClient) {
					delete MyClient;
					i=myClients.erase(i);
					break;
				}
			}
		}
		static void onClientMsg(const std::string& data, void* ClientPtr) {
			TCPAsyncClientSocket* MyClient = (TCPAsyncClientSocket*)ClientPtr;
			//std::cout << MyClient->getRemoteAddr().getIPString() << ":(" << data << ")" << std::endl;
			MyClient->SendMsg(data);
		}
		static void onClientError(int errCode, const std::string& errInfo, void* ClientPtr) {
			TCPAsyncClientSocket* MyClient = (TCPAsyncClientSocket*)ClientPtr;
			//std::cout << "ClientError:" << errInfo << std::endl;
		}
};
int main(int argc, char** args) {
	std::cout << "hi" << std::endl;
	
	EasyCrossPlatform::Network::Socket::TCPAsyncServerSocket mSocket;
	mSocket.Init();
	mSocket.ClientConnectCallBack = MyServerFunction::onClientConnect;
	mSocket.ClientDisconnectCallBack = MyServerFunction::onClientDisconnect;
	mSocket.ClientErrorCallBack = MyServerFunction::onClientError;
	mSocket.ClientMsgCallBack = MyServerFunction::onClientMsg;
	mSocket.ServerErrorCallBack = MyServerFunction::onServerError;
	mSocket.ServerNewConnCallBack = MyServerFunction::onServerNewConn;
	//Binding 0.0.0.0 means binding every interface.
	//Keep in mind that Linux users need to use root permission to bind the socket to 700. Otherwise you need to find a bigger number like 25535.
	mSocket.Listen(EasyCrossPlatform::Network::Socket::IpAddr("0.0.0.0", 700, true),200);
	//system("pause");
	std::cin.get();
	std::cout << "----" << std::endl;
	//If you want every computer in the LAN to recieve, type 255.255.255.255 for IP Address.
	std::cin.get();
	mSocket.StopListen();
	mSocket.Destroy();
	return 0;
}