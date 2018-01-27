#ifndef __EasyCrossPlatform_TCPSocketFile__
	#define __EasyCrossPlatform_TCPSocketFile__
	#include <EasyCP_Common.h>
	#include <XSYDMultiTask.h>
	#include <libuv/uv.h>
	#include <XSYDSocketResImpl.h>
	namespace EasyCrossPlatform{
		namespace Network{
			namespace Socket{
				typedef void(*TCPClientV4MsgCallBack)(const std::string& message, void* ClientClassPtr);
				typedef void(*TCPClientV4ConnectCallBack)(bool succeed, void* ClientClassPtr);
				typedef void(*TCPClientV4DisconnectCallBack)(void* ClientClassPtr);
				typedef void(*TCPServerV4NewConnectionCallBack)(TCPAsyncClientSocketv4* newClientSocket, void* ServerClassPtr);
				typedef void(*TCPClientV4ErrorCallBack)(int errCode, const std::string& errDescription, void* ClientClassPtr);
				typedef void(*TCPServerV4ErrorCallBack)(int errCode, const std::string& errDescription, void* ServerClassPtr);
				class TCPAsyncClientSocketv4 {
					friend class TCPAsyncServerSocketv4;
					private:
						
					protected:
						static std::map<uv_tcp_t*, std::vector<TCPAsyncClientSocketv4*>> m_MyClassPtrs;
						IpAddrV4 m_remoteAddr;
						std::shared_ptr<uv_tcp_t> m_ClientSocketHandle;
						bool m_Connected;
						bool Inited;
						bool Closing;
						std::mutex* myMutex;
						static void m_uv_connect_cb(uv_connect_t* req, int status);
						static void m_uv_close_cb(uv_handle_t* handle);
						static void m_uv_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
						static void m_uv_shutdown_cb(uv_shutdown_t* req, int state);
						static void m_uv_write_cb(uv_write_t* req, int status);
						static void m_uv_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

						void onConnected(bool Succeeded);
						void onMsg(const std::string& Msg);
						void onDisconnect();
						void onError(int errCode, const std::string& errDescription);
					public:
						TCPAsyncClientSocketv4();
						TCPAsyncClientSocketv4(const IpAddrV4& RemoteIP);
						TCPAsyncClientSocketv4(const TCPAsyncClientSocketv4& oldClient);
						
						void Init();
						void Connect();
						void setRemoteIPAddr(const IpAddrV4& newIP);
						IpAddrV4 getMyIpAddr();
						IpAddrV4 getRemoteAddr();
						void SendMsg(const std::string& Msg);
						void Disconnect();
						void Destroy();

						TCPClientV4ConnectCallBack ConnectCallBack;
						TCPClientV4MsgCallBack MsgCallBack;
						TCPClientV4ErrorCallBack ErrorCallBack;
						TCPClientV4DisconnectCallBack DisconnectCallBack;

						~TCPAsyncClientSocketv4();
				};

				class TCPAsyncServerSocketv4 {
					private:

					protected:
						void onConnection(TCPAsyncClientSocketv4* newClient);
						void onError(int errCode, const std::string& errorDescription);
						
						static std::map<uv_tcp_t*, std::vector<TCPAsyncServerSocketv4*>> m_MyClassPtrs;
						static void m_uv_connection_cb(uv_stream_t* server, int status);

						std::shared_ptr<uv_tcp_t> m_SocketHandle;
						IpAddrV4 m_myIP;
						bool isListening;
						bool hasInted;
						int m_QueueLength;
					public:
						TCPAsyncServerSocketv4();
						TCPAsyncServerSocketv4(const IpAddrV4& myIP, int QueLength);
						TCPAsyncServerSocketv4(const TCPAsyncServerSocketv4& oldServer);

						void Init();
						void Destroy();

						void setIP(const IpAddrV4& myIP, int QueLength = 0);
						IpAddrV4 getIP();
						void Listen();
						void Listen(const IpAddrV4& myIP, int QueLength = 0);
						void StopListen();

						TCPClientV4ConnectCallBack ClientConnectCallBack;
						TCPClientV4MsgCallBack ClientMsgCallBack;
						TCPClientV4ErrorCallBack ClientErrorCallBack;
						TCPClientV4DisconnectCallBack ClientDisconnectCallBack;
						TCPServerV4NewConnectionCallBack ServerNewConnCallBack;
						TCPServerV4ErrorCallBack ServerErrorCallBack;

						~TCPAsyncServerSocketv4();
				};

				typedef void(*TCPClientV6MsgCallBack)(const std::string& message, void* ClientClassPtr);
				typedef void(*TCPClientV6ConnectCallBack)(bool succeed, void* ClientClassPtr);
				typedef void(*TCPClientV6DisconnectCallBack)(void* ClientClassPtr);
				typedef void(*TCPServerV6NewConnectionCallBack)(TCPAsyncClientSocketv6* newClientSocket, void* ServerClassPtr);
				typedef void(*TCPClientV6ErrorCallBack)(int errCode, const std::string& errDescription, void* ClientClassPtr);
				typedef void(*TCPServerV6ErrorCallBack)(int errCode, const std::string& errDescription, void* ServerClassPtr);
				class TCPAsyncClientSocketv6 {
					friend class TCPAsyncServerSocketv6;
				private:

				protected:
					static std::map<uv_tcp_t*, std::vector<TCPAsyncClientSocketv6*>> m_MyClassPtrs;
					IpAddrV6 m_remoteAddr;
					std::shared_ptr<uv_tcp_t> m_ClientSocketHandle;
					bool m_Connected;
					bool Inited;
					bool Closing;
					std::mutex* myMutex;
					static void m_uv_connect_cb(uv_connect_t* req, int status);
					static void m_uv_close_cb(uv_handle_t* handle);
					static void m_uv_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
					static void m_uv_shutdown_cb(uv_shutdown_t* req, int state);
					static void m_uv_write_cb(uv_write_t* req, int status);
					static void m_uv_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

					void onConnected(bool Succeeded);
					void onMsg(const std::string& Msg);
					void onDisconnect();
					void onError(int errCode, const std::string& errDescription);
				public:
					TCPAsyncClientSocketv6();
					TCPAsyncClientSocketv6(const IpAddrV6& RemoteIP);
					TCPAsyncClientSocketv6(const TCPAsyncClientSocketv6& oldClient);

					void Init();
					void Connect();
					void setRemoteIPAddr(const IpAddrV6& newIP);
					IpAddrV6 getMyIpAddr();
					IpAddrV6 getRemoteAddr();
					void SendMsg(const std::string& Msg);
					void Disconnect();
					void Destroy();

					TCPClientV6ConnectCallBack ConnectCallBack;
					TCPClientV6MsgCallBack MsgCallBack;
					TCPClientV6ErrorCallBack ErrorCallBack;
					TCPClientV6DisconnectCallBack DisconnectCallBack;

					~TCPAsyncClientSocketv6();
				};

				class TCPAsyncServerSocketv6 {
				private:

				protected:
					void onConnection(TCPAsyncClientSocketv6* newClient);
					void onError(int errCode, const std::string& errorDescription);

					static std::map<uv_tcp_t*, std::vector<TCPAsyncServerSocketv6*>> m_MyClassPtrs;
					static void m_uv_connection_cb(uv_stream_t* server, int status);

					std::shared_ptr<uv_tcp_t> m_SocketHandle;
					IpAddrV6 m_myIP;
					bool isListening;
					bool hasInted;
					int m_QueueLength;
				public:
					TCPAsyncServerSocketv6();
					TCPAsyncServerSocketv6(const IpAddrV6& myIP, int QueLength);
					TCPAsyncServerSocketv6(const TCPAsyncServerSocketv6& oldServer);

					void Init();
					void Destroy();

					void setIP(const IpAddrV6& myIP, int QueLength = 0);
					IpAddrV6 getIP();
					void Listen();
					void Listen(const IpAddrV6& myIP, int QueLength = 0);
					void StopListen();

					TCPClientV6ConnectCallBack ClientConnectCallBack;
					TCPClientV6MsgCallBack ClientMsgCallBack;
					TCPClientV6ErrorCallBack ClientErrorCallBack;
					TCPClientV6DisconnectCallBack ClientDisconnectCallBack;
					TCPServerV6NewConnectionCallBack ServerNewConnCallBack;
					TCPServerV6ErrorCallBack ServerErrorCallBack;

					~TCPAsyncServerSocketv6();
				};
			}
		}
	}
#endif