#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;
std::vector<TCPAsyncClientSocket*> myClients;
std::mutex myMutex;
class MyServerFunction {
public:
	static void onServerNewConn(void* newClientSocket, void* ServerClassPtr) {
		TCPAsyncClientSocket* MyClientSocket = (TCPAsyncClientSocket*)newClientSocket;
		TCPAsyncServerSocket* MyServer = (TCPAsyncServerSocket*)ServerClassPtr;
		myClients.push_back(MyClientSocket);
	}
	static void onServerError(int errCode, const std::string& errInfo, void* ServerClassPtr) {
		TCPAsyncServerSocket* MyServer = (TCPAsyncServerSocket*)ServerClassPtr;
		std::cout << "SrvError:" << errInfo << std::endl;
	}
	static void onClientConnect(bool Succeed, void* ClientPtr) {
		TCPAsyncClientSocket* MyClient = (TCPAsyncClientSocket*)ClientPtr;
		//std::cout << "ClientConnect:" << MyClient->getRemoteAddr().getIPString() << ":" << MyClient->getRemoteAddr().getPort() << std::endl;
	}
	static void onClientDisconnect(void* ClientPtr) {
		TCPAsyncClientSocket* MyClient = (TCPAsyncClientSocket*)ClientPtr;
		MyClient->Destroy();

		myMutex.lock();
		bool ClientsEmpty = myClients.empty();


		if (!ClientsEmpty) {
			for (auto i = myClients.begin(); i != myClients.end(); i++) {
				if ((*i) == MyClient) {
					delete MyClient;
					i = myClients.erase(i);
					break;
				}
			}
		}
		myMutex.unlock();
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
	SocketWorker myListeningWorker;

	TCPAsyncServerSocket mSocket;
	//On Unix/Linux systems, libuv's server socket and the socket after connection established can be only served on one uv_loop_t handle.
	//We are now only accepting 1 SocketWorker for each Server Socket due to cross-platform concerns.
	mSocket.setWorkers(&myListeningWorker);
	mSocket.Init();
	mSocket.ClientConnectCallBack = MyServerFunction::onClientConnect;
	mSocket.ClientDisconnectCallBack = MyServerFunction::onClientDisconnect;
	mSocket.ClientErrorCallBack = MyServerFunction::onClientError;
	mSocket.ClientMsgCallBack = MyServerFunction::onClientMsg;
	mSocket.ServerErrorCallBack = MyServerFunction::onServerError;
	mSocket.ServerNewConnCallBack = MyServerFunction::onServerNewConn;
	//Binding 0.0.0.0 means binding every interface.
	//Keep in mind that Linux users need to use root permission to bind the socket to 700. Otherwise you need to find a bigger number like 25535(Port range from 1-65535).
	mSocket.Listen(IpAddr("0.0.0.0", 25565, true), 200);

	std::cin.get();
	std::cout << "----" << std::endl;
	//If you want every computer in the LAN to recieve, type 255.255.255.255 for IP Address.
	std::cin.get();
	mSocket.StopListen();
	mSocket.Destroy();
	return 0;
}