#ifndef __EASYCROSSPLATFORM_WEBSOCKETFILE__
	#define __EASYCROSSPLATFORM_WEBSOCKETFILE__
	#include <EasyCP_Common.h>
	#include <XSYDSocketResImpl.h>
	#include <XSYDStringUtil.h>
	#include <XSYDChrono.h>
	#include <XSYDEncryption.h>
	#include <XSYDHTTPParser.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace HTTPServer {
				class HTTPServer;
			}
			namespace Websocket {
				enum WebSocketFrameType {
					ERROR_FRAME = 0xFF00,
					INCOMPLETE_FRAME = 0xFE00,

					OPENING_FRAME = 0x3300,
					CLOSING_FRAME = 0x3400,

					INCOMPLETE_TEXT_FRAME = 0x01,
					INCOMPLETE_BINARY_FRAME = 0x02,

					TEXT_FRAME = 0x81,
					BINARY_FRAME = 0x82,

					PING_FRAME = 0x19,
					PONG_FRAME = 0x1A
				};

				class WebSocket
				{
				public:

					EasyCrossPlatform::Parser::HTTP::URL resource;
					std::string origin;
					std::string protocol;
					std::string key;

					WebSocket();

					/**
					* @param input_frame .in. pointer to input frame
					* @param input_len .in. length of input frame
					* @return [WS_INCOMPLETE_FRAME, WS_ERROR_FRAME, WS_OPENING_FRAME]
					*/
					WebSocketFrameType parseHandshake(const EasyCrossPlatform::Parser::HTTP::HTTPRequest& Request);
					std::string answerHandshake();

					int makeFrame(WebSocketFrameType frame_type, unsigned char* msg, int msg_len, unsigned char* buffer, int buffer_len);
					WebSocketFrameType getFrame(unsigned char* in_buffer, int& in_length, unsigned char* out_buffer, int out_size, int* out_length);

					std::string trim(std::string str);
					std::vector<std::string> explode(std::string theString, std::string theDelimiter, bool theIncludeEmptyStrings = false);
				};

				class WebsocketSingleConnection;
				typedef void (*WebsocketLatencyTestCallBack)(double Latency, WebsocketSingleConnection*);
				class WebsocketSingleConnection : public Socket::StandardClientSocket {
				friend class EasyCrossPlatform::Network::HTTPServer::HTTPServer;
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
					bool m_lastMsgIsBinary = false;
					WebSocket mParser;
					bool checkNewMsg();
				public:
					WebsocketSingleConnection(const EasyCrossPlatform::Parser::HTTP::HTTPRequest& HandshakeRequest, Socket::StandardClientSocket* ClientSocket, bool DeleteSocketAfter = true);
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
					EasyCrossPlatform::Parser::HTTP::URL WebsocketURL;
					bool lastMsgIsBinary();
					~WebsocketSingleConnection();
				};
			}
		}
	}
#endif