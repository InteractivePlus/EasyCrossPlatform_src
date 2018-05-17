#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;
std::vector<TLS::TLSSNIAsyncServerSingleConnection*> myClients;
class MyTLSFunctions {
public:
	static void TLSClientMsgCallBack(const std::vector<byte>& Msg, StandardClientSocket* ClientPtr) {
		TLS::TLSSNIAsyncServerSingleConnection* mClientSocket = (TLS::TLSSNIAsyncServerSingleConnection*) ClientPtr;
		mClientSocket->SendMsg(Msg);
		std::cout << EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg) << std::endl;
	}
	static void TLSClientConnectCallBack(bool Succeed, StandardClientSocket* ClientPtr) {
		TLS::TLSSNIAsyncServerSingleConnection* mClientSocket = (TLS::TLSSNIAsyncServerSingleConnection*) ClientPtr;
		std::cout << "Sock Connect " << (Succeed ? "succeeded" : "failed") << std::endl;
		std::cout << "SNI:" << mClientSocket->serverHostName << std::endl;
	}
	static void TLSClientDisconnectCallBack(StandardClientSocket* ClientPtr) {
		TLS::TLSSNIAsyncServerSingleConnection* mClientSocket = (TLS::TLSSNIAsyncServerSingleConnection*) ClientPtr;
		std::cout << "Sock Disconnect" << std::endl;
		for (auto i = myClients.begin(); i != myClients.end(); i++) {
			if ((*i) == mClientSocket) {
				delete mClientSocket;
				i = myClients.erase(i);
				break;
			}
		}
	}
	static void TLSServerNewConnectionCallBack(StandardClientSocket* NewClientPtr, StandardServerSocket* ServerPtr) {
		TLS::TLSSNIAsyncServerSingleConnection* mClientSocket = (TLS::TLSSNIAsyncServerSingleConnection*) NewClientPtr;
		TLS::TLSSNIAsyncServer* mServerSocket = (TLS::TLSSNIAsyncServer*) ServerPtr;
		std::cout << "New Conn Established!" << std::endl;
		myClients.push_back(mClientSocket);
	}
	static void TLSClientErrorCallBack(int ErrNo, const std::string& ErrDescription, StandardClientSocket* ClientPtr) {
		TLS::TLSSNIAsyncServerSingleConnection* mClientSocket = (TLS::TLSSNIAsyncServerSingleConnection*) ClientPtr;
		std::cout << "Client Err:[" << std::to_string(ErrNo) << "](" << ErrDescription << ")" << std::endl;
	}
	static void TLSServerErrorCallBack(int ErrNo, const std::string& ErrDescription, StandardServerSocket* SrvPtr) {
		TLS::TLSSNIAsyncServer* mServerSocket = (TLS::TLSSNIAsyncServer*) SrvPtr;
		std::cout << "Server Err:[" << std::to_string(ErrNo) << "](" << ErrDescription << ")" << std::endl;
	}
};
int main(int argc, char** args) {

	SocketWorker mWorker;
	std::map<std::string, std::pair<std::string, std::pair<std::string, std::string>>> mSrvCerts;
	//Reading Server Crt Files
	mSrvCerts["default"] = std::pair<std::string, std::pair<std::string, std::string>>(EasyCrossPlatform::File::FileIO::ReadFile(EasyCrossPlatform::Runtime::Path::APPPath() + "XSYDGW.crt"), std::pair<std::string, std::string>(EasyCrossPlatform::File::FileIO::ReadFile(EasyCrossPlatform::Runtime::Path::APPPath() + "XSYDGW.key"), ""));
	//std::pair<Cert,std::pair<Private Key, Key Password> is the stored value in the data.
	//default cert is shown to the client when no domain is indicated or the domain is not in the list
	IpAddr BindingIP = IpAddr("0.0.0.0", 25566U);

	//Setting up
	TLS::TLSSNIAsyncServer mServer = TLS::TLSSNIAsyncServer(BindingIP, &mWorker, mSrvCerts);
	mServer.SetClientConnectCallBack(MyTLSFunctions::TLSClientConnectCallBack);
	mServer.SetClientDisconnectCallBack(MyTLSFunctions::TLSClientDisconnectCallBack);
	mServer.SetServerNewConnCallBack(MyTLSFunctions::TLSServerNewConnectionCallBack);
	mServer.SetClientErrorCallBack(MyTLSFunctions::TLSClientErrorCallBack);
	mServer.SetClientMsgCallBack(MyTLSFunctions::TLSClientMsgCallBack);
	mServer.SetServerErrorCallBack(MyTLSFunctions::TLSServerErrorCallBack);

	std::cout << "Press any key to start Listen" << std::endl;
	std::cin.get();
	mServer.StartListen();
	std::cout << "Press any key to stop listen" << std::endl;
	std::cin.get();
	mServer.StopListen();
	return 0;
}