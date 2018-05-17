#ifndef __EASYCROSSPLATFORM_WEBSOCKETFILE__
	#define __EASYCROSSPLATFORM_WEBSOCKETFILE__
	#include <EasyCP_Common.h>
	#include <XSYDSocketResImpl.h>
	#include <XSYDStringUtil.h>
	#include <XSYDChrono.h>
	#include <websocketpp/WebSocket.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Websocket {
				class WebsocketSingleConnection;
				typedef void (*WebsocketLatencyTestCallBack)(double Latency, WebsocketSingleConnection*);
				class WebsocketSingleConnection : public Socket::StandardClientSocket {
				private:
					WebsocketLatencyTestCallBack m_LatencyCallBack;
					Socket::StandardClientConnCallBack m_ConnectCallBack;
					Socket::StandardClientMsgCallBack m_MsgCallBack;
					Socket::StandardClientDisconnectCallBack m_DisconnCallBack;
					Socket::StandardClientErrorCallBack m_ErrCallBack;
				protected:
					void Init();
					void Destroy();

					std::deque<byte> BufferWaitingForRead;
					bool m_Shaked = false;
					bool m_deleteSocketAfter = false;

					double PongFrameTime = 0.0;

					void onConnect(bool Succeed);
					void onMsg(const std::string& Msg);
					void onDisconn();
					void onError(int errNo, const std::string& errDescription);
					void onPong();

					Socket::StandardClientSocket* m_SocketHandle = NULL;

					static void m_Socket_Connection(bool Succeed, Socket::StandardClientSocket* SocketPtr);
					static void m_Socket_Msg(const std::vector<byte>& Msg, Socket::StandardClientSocket* SocketPtr);
					static void m_Socket_Disconn(Socket::StandardClientSocket* SocketPtr);
					static void m_Socket_Error(int errNo, const std::string& errDescription, Socket::StandardClientSocket* SocketPtr);

					WebSocket mParser;
					bool checkNewMsg();
				public:
					WebsocketSingleConnection(const std::string& HandshakeRequest, Socket::StandardClientSocket* ClientSocket, bool DeleteSocketAfter = true);
					void SetConnectCallBack(Socket::StandardClientConnCallBack mCB);
					void SetMsgCallBack(Socket::StandardClientMsgCallBack mCB);
					void SetDisconnectCallBack(Socket::StandardClientDisconnectCallBack mCB);
					void SetErrorCallBack(Socket::StandardClientErrorCallBack mCB);
					void SetLatencyTestCallBack(WebsocketLatencyTestCallBack mCB);
					void TestLatency();
					void SendMsg(const std::string& Msg);
					void SendMsg(const std::vector<byte>& Msg);
					void Connect();
					void Disconnect();
					bool isConnected();
					~WebsocketSingleConnection();
				};
			}
		}
	}
#endif