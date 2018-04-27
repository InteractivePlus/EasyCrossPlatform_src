#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network;
std::vector<Socket::TLSSNIAsyncServerSingleConnection*> myClients;
class MyTLSFunctions {
public:
	static void TLSClientMsgCallBack(const std::vector<byte>& Msg, void* ClientPtr) {
		Socket::TLSSNIAsyncServerSingleConnection* mClientSocket = (Socket::TLSSNIAsyncServerSingleConnection*) ClientPtr;
		mClientSocket->SendMsg(Msg);
		std::cout << EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg) << std::endl;
	}
	static void TLSClientConnectCallBack(bool Succeed, void* ClientPtr) {
		Socket::TLSSNIAsyncServerSingleConnection* mClientSocket = (Socket::TLSSNIAsyncServerSingleConnection*) ClientPtr;
		std::cout << "Sock Connect " << (Succeed ? "succeeded" : "failed") << std::endl;
		std::cout << "SNI:" << mClientSocket->serverHostName << std::endl;
	}
	static void TLSClientDisconnectCallBack(void* ClientPtr) {
		Socket::TLSSNIAsyncServerSingleConnection* mClientSocket = (Socket::TLSSNIAsyncServerSingleConnection*) ClientPtr;
		std::cout << "Sock Disconnect" << std::endl;
		for (auto i = myClients.begin(); i != myClients.end(); i++) {
			if ((*i) == mClientSocket) {
				delete mClientSocket;
				i = myClients.erase(i);
				break;
			}
		}
	}
	static void TLSServerNewConnectionCallBack(void* NewClientPtr, void* ServerPtr) {
		Socket::TLSSNIAsyncServerSingleConnection* mClientSocket = (Socket::TLSSNIAsyncServerSingleConnection*) NewClientPtr;
		Socket::TLSSNIAsyncServer* mServerSocket = (Socket::TLSSNIAsyncServer*) ServerPtr;
		std::cout << "New Conn Established!" << std::endl;
		myClients.push_back(mClientSocket);
	}
	static void TLSClientErrorCallBack(int ErrNo, const std::string& ErrDescription, void* ClientPtr) {
		Socket::TLSSNIAsyncServerSingleConnection* mClientSocket = (Socket::TLSSNIAsyncServerSingleConnection*) ClientPtr;
		std::cout << "Client Err:[" << std::to_string(ErrNo) << "](" << ErrDescription << ")" << std::endl;
	}
	static void TLSServerErrorCallBack(int ErrNo, const std::string& ErrDescription, void* SrvPtr) {
		Socket::TLSSNIAsyncServer* mServerSocket = (Socket::TLSSNIAsyncServer*) SrvPtr;
		std::cout << "Server Err:[" << std::to_string(ErrNo) << "](" << ErrDescription << ")" << std::endl;
	}
};
int main(int argc, char** args) {
	Socket::TLSSNIAsyncServer mServer;
	Socket::SocketWorker mWorker;
	std::map<std::string, std::pair<std::string, std::pair<std::string,std::string>>> mSrvCerts;
	//Reading Server Crt Files
	mSrvCerts["default"] = std::pair<std::string,std::pair<std::string,std::string>>(EasyCrossPlatform::File::FileIO::ReadFile(EasyCrossPlatform::Runtime::Path::APPPath() + "XSYDGW.crt"), std::pair<std::string,std::string>(EasyCrossPlatform::File::FileIO::ReadFile(EasyCrossPlatform::Runtime::Path::APPPath() + "XSYDGW.key"),""));
	//std::pair<Cert,std::pair<Private Key, Key Password>
	
	//Setting up
	mServer.ConnectCallBack = MyTLSFunctions::TLSClientConnectCallBack;
	mServer.DisconnectCallBack = MyTLSFunctions::TLSClientDisconnectCallBack;
	mServer.ConnectionCallback = MyTLSFunctions::TLSServerNewConnectionCallBack;
	mServer.ErrorCallBack = MyTLSFunctions::TLSClientErrorCallBack;
	mServer.MsgCallBack = MyTLSFunctions::TLSClientMsgCallBack;
	mServer.SrvErrorCallBack = MyTLSFunctions::TLSServerErrorCallBack;
	mServer.setIP(Socket::IpAddr("0.0.0.0", 25566, true),200);
	mServer.setWorker(&mWorker);
	mServer.setServerCerts(mSrvCerts);
	mServer.setCACerts(EasyCrossPlatform::File::FileIO::ReadFile(EasyCrossPlatform::Runtime::Path::APPPath() + "CALIST.txt"));
	mServer.Init();
	mServer.Listen();
	system("pause");
	mServer.Destroy();
}