#include <EasyCrossPlatform.h>
#include <iostream>

class MySocket : public EasyCrossPlatform::Network::Socket::TCPAsyncClientSocketv4{
	public:
	//Inherit from Constructor, You can choose not to inherit constructor as long as you use Class.Init() and Class.Destroy() in your code.
	MySocket() : TCPAsyncClientSocketv4() {}
	MySocket(const EasyCrossPlatform::Network::Socket::IpAddrV4& RemoteIP) : TCPAsyncClientSocketv4(RemoteIP) {}
	MySocket(MySocket& oldSocket) : TCPAsyncClientSocketv4(oldSocket) {}
	~MySocket() { TCPAsyncClientSocketv4::~TCPAsyncClientSocketv4(); }
	//Inherit Finish

	static void ConnectCB(bool Succeeded, EasyCrossPlatform::Network::Socket::TCPAsyncClientSocketv4* ClassPtr){
		std::cout << "Socket Connected" << std::endl;
	}
	static void MsgCB(const std::string& Msg, EasyCrossPlatform::Network::Socket::TCPAsyncClientSocketv4* ClassPtr){
		std::cout << "SocketMsg:" << Msg << std::endl;
	}
	void DisconnectCB(EasyCrossPlatform::Network::Socket::TCPAsyncClientSocketv4* ClassPtr){
		std::cout << "SocketDis" << std::endl;
	}
	void ErrorCB(int errCode, const std::string& errDescription, EasyCrossPlatform::Network::Socket::TCPAsyncClientSocketv4* ClassPtr){
		std::cout << "Error:" << errDescription << std::endl;
	}
};
int main(int argc, char** args) {
	std::cout << "hi" << std::endl;
	
	MySocket mSocket;
	mSocket.Init();
	//Set CallBack
	mSocket.ConnectCallBack = MySocket::ConnectCB;
	mSocket.MsgCallBack = MySocket::MsgCB;
	mSocket.DisconnectCallBack = MySocket::DisconnectCB;
	mSocket.ErrorCallBack = MySocket::ErrorCB;
	//Set CallBack finish.
	mSocket.setRemoteIPAddr(EasyCrossPlatform::Network::Socket::IpAddrV4("127.0.0.1",700));
	mSocket.Connect();
	system("pause");
	mSocket.SendMsg("2333");
	system("pause");
	mSocket.Disconnect();
	mSocket.Destroy();
	return 0;
}