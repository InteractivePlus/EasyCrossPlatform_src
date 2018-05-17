#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;
std::vector<TCP::TCPAsyncClientSocket*> myClients;
std::mutex myMutex;
class MyServerFunction {
public:
	static void onServerNewConn(StandardClientSocket* newClientSocket, StandardServerSocket* ServerClassPtr) {
		TCP::TCPAsyncClientSocket* MyClientSocket = (TCP::TCPAsyncClientSocket*) newClientSocket;
		TCP::TCPAsyncServerSocket* MyServer = (TCP::TCPAsyncServerSocket*)ServerClassPtr;
		myClients.push_back(MyClientSocket);
	}
	static void onServerError(int errCode, const std::string& errInfo, StandardServerSocket* ServerClassPtr) {
		TCP::TCPAsyncServerSocket* MyServer = (TCP::TCPAsyncServerSocket*) ServerClassPtr;
		std::cout << "SrvError:" << errInfo << std::endl;
	}
	static void onClientConnect(bool Succeed, StandardClientSocket* ClientPtr) {
		TCP::TCPAsyncClientSocket* MyClient = (TCP::TCPAsyncClientSocket*) ClientPtr;
		//std::cout << "ClientConnect:" << MyClient->getRemoteAddr().getIPString() << ":" << MyClient->getRemoteAddr().getPort() << std::endl;
	}
	static void onClientDisconnect(StandardClientSocket* ClientPtr) {
		TCP::TCPAsyncClientSocket* MyClient = (TCP::TCPAsyncClientSocket*) ClientPtr;

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
	static void onClientMsg(const std::vector<byte>& data, StandardClientSocket* ClientPtr) {
		TCP::TCPAsyncClientSocket* MyClient = (TCP::TCPAsyncClientSocket*) ClientPtr;
		//std::cout << MyClient->getRemoteAddr().getIPString() << ":(" << data << ")" << std::endl;
		MyClient->SendMsg(data);
	}
	static void onClientError(int errCode, const std::string& errInfo, StandardClientSocket* ClientPtr) {
		TCP::TCPAsyncClientSocket* MyClient = (TCP::TCPAsyncClientSocket*) ClientPtr;
		//std::cout << "ClientError:" << errInfo << std::endl;
	}
};
int main(int argc, char** args) {
	SocketWorker myListeningWorker;
	IpAddr ListeningAddr = IpAddr("::", 25565U); //Listening on :: or 0.0.0.0 means listen on all network addresses.
	TCP::TCPAsyncServerSocket mSocket = TCP::TCPAsyncServerSocket(ListeningAddr, &myListeningWorker);
	//On Unix/Linux systems, libuv's server socket and the socket after connection established can be only served on one uv_loop_t handle.
	//We are now only accepting 1 SocketWorker for each Server Socket due to cross-platform concerns.
	mSocket.SetClientConnectCallBack(MyServerFunction::onClientConnect);
	mSocket.SetClientDisconnectCallBack(MyServerFunction::onClientDisconnect);
	mSocket.SetClientErrorCallBack(MyServerFunction::onClientError);
	mSocket.SetClientMsgCallBack(MyServerFunction::onClientMsg);
	mSocket.SetServerErrorCallBack(MyServerFunction::onServerError);
	mSocket.SetServerNewConnCallBack(MyServerFunction::onServerNewConn);
	//Binding 0.0.0.0 means binding every interface.
	//Keep in mind that Linux users need to use root permission to bind the socket to 700. Otherwise you need to find a bigger number like 25535(Port range from 1-65535).
	std::cout << "Press any key to start listen" << std::endl;
	std::cin.get();
	mSocket.StartListen();
	std::cout << "Listen started" << std::endl;

	std::cout << "----" << std::endl;
	std::cout << "Press any key to stop listen" << std::endl;
	std::cin.get();
	mSocket.StopListen();
	return 0;
}