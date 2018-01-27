#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;
std::vector<TCPAsyncClientSocketv4*> myClients;
class MyServerFunction{
	public:
		static void onServerNewConn(TCPAsyncClientSocketv4* newClientSocket, void* ServerClassPtr) {
			TCPAsyncServerSocketv4* MyServer = (TCPAsyncServerSocketv4*)ServerClassPtr;
			myClients.push_back(newClientSocket);
		}
		static void onServerError(int errCode, const std::string& errInfo, void* ServerClassPtr) {
			TCPAsyncServerSocketv4* MyServer = (TCPAsyncServerSocketv4*)ServerClassPtr;
			//std::cout << "SrvError:" << errInfo << std::endl;
		}
		static void onClientConnect(bool Succeed, void* ClientPtr){
			TCPAsyncClientSocketv4* MyClient = (TCPAsyncClientSocketv4*)ClientPtr;
			
			//std::cout << "ClientConnect:" << MyClient->getRemoteAddr().getIPString() << ":" << MyClient->getRemoteAddr().getPort() << std::endl;
		}
		static void onClientDisconnect(void* ClientPtr) {
			TCPAsyncClientSocketv4* MyClient = (TCPAsyncClientSocketv4*)ClientPtr;
			//std::cout << "ClientDisconnect:" << std::endl;
			MyClient->Destroy();
			for (auto i = myClients.begin(); i != myClients.end(); i++) {
				if ((*i) == MyClient) {
					free(MyClient);
					i=myClients.erase(i);
					break;
				}
			}
		}
		static void onClientMsg(const std::string& data, void* ClientPtr) {
			TCPAsyncClientSocketv4* MyClient = (TCPAsyncClientSocketv4*)ClientPtr;
			//std::cout << MyClient->getRemoteAddr().getIPString() << ":(" << data << ")" << std::endl;
			MyClient->SendMsg(data);
		}
		static void onClientError(int errCode, const std::string& errInfo, void* ClientPtr) {
			TCPAsyncClientSocketv4* MyClient = (TCPAsyncClientSocketv4*)ClientPtr;
			//std::cout << "ClientError:" << errInfo << std::endl;
		}
};
int main(int argc, char** args) {
	std::cout << "hi" << std::endl;
	
	EasyCrossPlatform::Network::Socket::TCPAsyncServerSocketv4 mSocket;
	mSocket.Init();
	mSocket.ClientConnectCallBack = MyServerFunction::onClientConnect;
	mSocket.ClientDisconnectCallBack = MyServerFunction::onClientDisconnect;
	mSocket.ClientErrorCallBack = MyServerFunction::onClientError;
	mSocket.ClientMsgCallBack = MyServerFunction::onClientMsg;
	mSocket.ServerErrorCallBack = MyServerFunction::onServerError;
	mSocket.ServerNewConnCallBack = MyServerFunction::onServerNewConn;
	//Binding 0.0.0.0 means binding every interface.
	mSocket.Listen(EasyCrossPlatform::Network::Socket::IpAddrV4("0.0.0.0", 700),200);
	//system("pause");
	std::cin.get();
	std::cout << "----" << std::endl;
	//If you want every computer in the LAN to recieve, type 255.255.255.255 for IP Address.
	std::cin.get();
	mSocket.StopListen();
	mSocket.Destroy();
	return 0;
}