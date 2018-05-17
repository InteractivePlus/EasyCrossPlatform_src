#ifndef __EasyCrossPlatform_TCPSocketFile__
	#define __EasyCrossPlatform_TCPSocketFile__
	#include <EasyCP_Common.h>
	#include <XSYDMultiTask.h>
	#include <libuv/uv.h>
	#include <XSYDSocketResImpl.h>
	#include <XSYDStringUtil.h>
	namespace EasyCrossPlatform{
		namespace Network{
			namespace Socket{
				namespace TCP {
					class TCPAsyncClientSocket;
					class TCPAsyncServerSocket;

					class TCPAsyncClientSocket : public StandardClientSocket {
						friend class TCPAsyncServerSocket;
					private:
						StandardClientConnCallBack m_ConnectCallBack = NULL;
						StandardClientMsgCallBack m_MsgCallBack = NULL;
						StandardClientErrorCallBack m_ErrorCallBack = NULL;
						StandardClientDisconnectCallBack m_DisconnectCallBack = NULL;

					protected:
						TCPAsyncClientSocket(SocketWorker* mWorker);
						IpAddr m_remoteAddr;
						uv_tcp_t* m_ClientSocketHandle;
						bool m_Connected = false;
						bool Closing = false;
						static void m_uv_connect_cb(uv_connect_t* req, int status);
						static void m_uv_close_cb(uv_handle_t* handle);
						static void m_uv_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
						static void m_uv_shutdown_cb(uv_shutdown_t* req, int state);
						static void m_uv_write_cb(uv_write_t* req, int status);
						static void m_uv_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

						SocketWorker* m_SocketWorker = NULL;
						void onConnected(bool Succeeded);
						void onMsg(const std::vector<byte>& Msg);
						void onDisconnect();
						void onError(int errCode, const std::string& errDescription);

						void Init();
						void Destroy();
					public:
						TCPAsyncClientSocket(const IpAddr& RemoteIP, SocketWorker* mWorker, unsigned short SelfPortU);
						TCPAsyncClientSocket(const TCPAsyncClientSocket& oldClient);
						void SetConnectCallBack(StandardClientConnCallBack mCallBack);
						void SetMsgCallBack(StandardClientMsgCallBack mCallBack);
						void SetDisconnectCallBack(StandardClientDisconnectCallBack mCallBack);
						void SetErrorCallBack(StandardClientErrorCallBack mCallBack);

						void Connect();
						IpAddr getMyIpAddr();
						IpAddr getRemoteAddr();
						bool isConnected();
						void SendMsg(const std::string& Msg);
						void SendMsg(const std::vector<byte>& Msg);
						void Disconnect();



						~TCPAsyncClientSocket();
					};

					class TCPAsyncServerSocket : public StandardServerSocket {
					private:
						StandardClientConnCallBack m_ClientConnectCallBack;
						StandardClientMsgCallBack m_ClientMsgCallBack;
						StandardClientErrorCallBack m_ClientErrorCallBack;
						StandardClientDisconnectCallBack m_ClientDisconnectCallBack;
						StandardServerNewConnectionCallBack m_ServerNewConnCallBack;
						StandardServerErrorCallBack m_ServerErrorCallBack;
					protected:
						void onConnection(TCPAsyncClientSocket* newClient);
						void onError(int errCode, const std::string& errorDescription);

						void Init();
						void Destroy();

						static void m_uv_connection_cb(uv_stream_t* server, int status);

						uv_tcp_t m_SocketHandle;
						IpAddr m_myIP;
						bool m_isListening = false;
						int m_QueueLength;
						SocketWorker* myListenWorker = NULL;
					public:
						TCPAsyncServerSocket(const IpAddr& myIP, SocketWorker* mWorker, int QueLength = 500);
						TCPAsyncServerSocket(const TCPAsyncServerSocket& oldServer);

						void SetClientConnectCallBack(StandardClientConnCallBack mCB);
						void SetClientMsgCallBack(StandardClientMsgCallBack mCB);
						void SetClientDisconnectCallBack(StandardClientDisconnectCallBack mCB);
						void SetClientErrorCallBack(StandardClientErrorCallBack mCB);
						void SetServerNewConnCallBack(StandardServerNewConnectionCallBack mCB);
						void SetServerErrorCallBack(StandardServerErrorCallBack mCB);

						IpAddr getIP();
						void StartListen();
						void StopListen();
						bool isListening();

						~TCPAsyncServerSocket();
					};
				}
			}
		}
	}
#endif