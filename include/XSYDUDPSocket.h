#ifndef __EasyCrossPlatform_UDPSocketFile__
	#define __EasyCrossPlatform_UDPSocketFile__
	#include <EasyCP_Common.h>
	#include <libuv/uv.h>
	#include <XSYDMultiTask.h>
	#include <XSYDSocketResImpl.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Socket {
				class UDPAsyncSocket {
					private:

					protected:
						static std::map<uv_udp_t*, std::vector<UDPAsyncSocket*>> m_MyClassPtrs;

						static void m_uv_read_cb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* remoteaddr, unsigned flags);
						static void m_uv_send_cb(uv_udp_send_t* req, int status);
						static void m_uv_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

						std::shared_ptr<uv_udp_t> m_SocketHandle;
						bool m_isListenMode;
						bool m_isListening;
						bool m_hasInited;

						IpAddr m_myRemoteAddr;
						IpAddr m_myAddr;
					public:
						UDPAsyncSocket();
						UDPAsyncSocket(const IpAddr& ListeningAddr);
						UDPAsyncSocket(UDPAsyncSocket& oldUDP);

						void setmyIP(const IpAddr& myNewIP);
						void sendMsg(const IpAddr& Destination,const std::string& Data);
						void Listen();
						void Listen(const IpAddr& myNewAddr);
						void StopListen();

						void Init();
						void Destroy();

						virtual void onMsg(const IpAddr& SourceIP,const std::string& data);
						virtual void onError(int errCode, const std::string& errDescription);

						~UDPAsyncSocket();
				};
			}
		}
	}
#endif