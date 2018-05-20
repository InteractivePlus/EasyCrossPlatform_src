#ifndef __EASYCROSSPLATFORM_FILE_HTTPSERVER__
	#define __EASYCROSSPLATFORM_FILE_HTTPSERVER__
	#include <EasyCP_Common.h>
	#include <XSYDHTTPParser.h>
	#include <XSYDWebSocket.h>
	#include <XSYDSocketResImpl.h>
	#include <XSYDMultiTask.h>
	#include <XSYDChrono.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace HTTPServer {
				const constexpr unsigned int MinCompressMsgLength = 1024U;
				class HTTPServer;
				struct HTTPRequestClientInfo {
					float ConnectionStartTime;
					std::string RemainingMsg;
					std::mutex* MsgMutex;
					Socket::StandardClientSocket* SocketPtr;
				};

				typedef void(*HTTPServerOnRequestCallBack)(const Parser::HTTP::HTTPRequest&, Parser::HTTP::HTTPResponse&);
				
				struct CallOnRequestPara {
					Socket::StandardClientSocket* SocketPtr;
					HTTPServer* ServerPtr;
				};

				class HTTPServer : public Socket::CustomDataCarrier {
				private:
					HTTPServerOnRequestCallBack m_RequestCallBack = NULL;
					Socket::StandardClientConnCallBack m_WebsocketConnectCallBack = NULL;
					Socket::StandardClientDisconnectCallBack m_WebsocketDisconnectCallBack = NULL;
					Socket::StandardClientMsgCallBack m_WebsocketMsgCallBack = NULL;
					Socket::StandardClientErrorCallBack m_WebsocketErrCallBack = NULL;
					Websocket::WebsocketLatencyTestCallBack m_WebsocketLatencyCallBack = NULL;
				protected:
					void Init();
					void Destroy();

					float m_MaxConnectionAliveTime = 0.0F;

					std::map<Socket::StandardClientSocket*, HTTPRequestClientInfo> m_NormalHTTPConnInfo;
					std::mutex* ConnectInfoMutex;
					Socket::StandardServerSocket* m_ServerSocket = NULL;
					static void TCPConnectCallBack(bool Succeed, Socket::StandardClientSocket* ClientSocketPtr);
					static void TCPDisconnectCallBack(Socket::StandardClientSocket* ClientSocketPtr);
					static void TCPMsgCallBack(const std::vector<byte>& Data, Socket::StandardClientSocket* ClientSocketPtr);
					static void TCPErrorCallBack(int ErrNo, const std::string& ErrDescription, Socket::StandardClientSocket* ClientSocketPtr);
					static void TCPConnectionCallBack(Socket::StandardClientSocket* newClient, Socket::StandardServerSocket* ServerSocket);
					static void TCPSRVErrorCallBack(int errNo, const std::string& errDescription, Socket::StandardServerSocket* ServerSocket);
					static void CallOnRequest(std::thread::id ThreadID, void* Parameters, bool * RunningSign, std::mutex *Mutex);
					static bool CheckMsg(HTTPServer* myServer, Socket::StandardClientSocket* SocketPtr);
					static void SuperviseMaxConnTime(std::thread::id ThreadID, void* Parameters, bool* RunningSign, std::mutex* Mutex);
					Thread::WorkPool* m_WorkerPool = NULL;
					Thread::SingleWork* AliveTimeSupervisor = NULL;
				public:
					HTTPServer(Socket::StandardServerSocket* ServerSocketPtr, unsigned int WorkerNumber = 1U, float MaxConnAliveTime = 120.0F);
					void setWebsocketCallBacks(Socket::StandardClientConnCallBack ConnectionCallBack = NULL, Socket::StandardClientDisconnectCallBack DisconnectCallBack = NULL, Socket::StandardClientErrorCallBack ErrorCallBack = NULL, Socket::StandardClientMsgCallBack MsgCallBack = NULL, Websocket::WebsocketLatencyTestCallBack LatencyCallBack = NULL);
					void setOnRequestCallBack(HTTPServerOnRequestCallBack onReqCallBack = NULL);
					bool AcceptWebsocket = true;
					void StartListen();
					void StopListen();
					bool isListening();
					~HTTPServer();
				};
			}
		}
	}
#endif