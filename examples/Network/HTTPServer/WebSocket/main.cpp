#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Network::Socket;

std::vector<StandardClientSocket*> mWebsockets;
class MyHTTPResponse {
public:
	static void OnResponse(StandardClientSocket* ClientSocket, const EasyCrossPlatform::Parser::HTTP::HTTPRequest& RequestClass, EasyCrossPlatform::Parser::HTTP::HTTPResponse& ResponseClass) {
		//You dont need to control Socket Behavior here but ClientSocket is a information for you to check negotiated ALPN, serverHost for TLS.
		ResponseClass.OriginalData = "<!DOCTYPE HTML><html><title>Websocket Test</title><head></head><body><h1>This is a test of websocket, please access this page using websocket.</h1></body></html>";
		ResponseClass.ResponseCode = 200U;
		ResponseClass.ResponseDescription = "OK";
	}
	static void OnWebsocketConnect(bool Succeed, StandardClientSocket* mSocketPtr) {
		mWebsockets.push_back(mSocketPtr);
		std::cout << "Connect" << std::endl;
	}
	static void OnWebsocketMsg(const std::vector<byte>& Msg, StandardClientSocket* mSocketPtr) {
		EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection* mWebsocket = (EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection*) mSocketPtr;
		std::cout << "Msg:" << EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg) << std::endl;
		if (mWebsocket->lastMsgIsBinary()) {
			mSocketPtr->SendMsg(Msg);
		}
		else {
			mSocketPtr->SendMsg(EasyCrossPlatform::Parser::StringUtil::fromBytes(Msg));
		}
	}
	static void OnWebsocketDisconnect(StandardClientSocket* mSocketPtr) {
		for (auto i = mWebsockets.begin(); i != mWebsockets.end(); i++) {
			if ((*i) == mSocketPtr) {
				delete mSocketPtr;
				i = mWebsockets.erase(i);
				break;
			}
		}
		std::cout << "Disconnect" << std::endl;
	}
	static void OnWebsocketError(int errNo, const std::string& errDescription, StandardClientSocket* mSocketPtr) {
		std::cout << "Error:" << errDescription << std::endl;
	}
};
int main(int argc, char** args) {
	IpAddr ListeningAddr = IpAddr("0.0.0.0", 25565U);
	SocketWorker mWorker;
	TCP::TCPAsyncServerSocket mServer = TCP::TCPAsyncServerSocket(ListeningAddr, &mWorker, 500);
	EasyCrossPlatform::Network::HTTPServer::HTTPServer mHTTPServer = EasyCrossPlatform::Network::HTTPServer::HTTPServer(&mServer, 1U, 120.0F);
	mHTTPServer.AcceptWebsocket = true;
	mHTTPServer.setWebsocketCallBacks(MyHTTPResponse::OnWebsocketConnect, MyHTTPResponse::OnWebsocketDisconnect, MyHTTPResponse::OnWebsocketError, MyHTTPResponse::OnWebsocketMsg, NULL);
	mHTTPServer.setOnRequestCallBack(MyHTTPResponse::OnResponse);
	std::cout << "Press any key to start listen" << std::endl;
	std::cin.get();
	mHTTPServer.StartListen();
	std::cout << "Press any key to exit" << std::endl;
	std::cin.get();
	mHTTPServer.StopListen();
	return 0;
}