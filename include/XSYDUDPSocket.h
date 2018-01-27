#ifndef __EasyCrossPlatform_UDPSocketFile__
	#define __EasyCrossPlatform_UDPSocketFile__
	#include <EasyCP_Common.h>
	#include <libuv/uv.h>
	#include <XSYDMultiTask.h>
	#include <XSYDSocketResImpl.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Socket {
				class UDPAsyncSocketv4 {
					private:

					protected:
						static std::map<uv_udp_t*, std::vector<UDPAsyncSocketv4*>> m_MyClassPtrs;

						static void m_uv_read_cb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* remoteaddr, unsigned flags);
						static void m_uv_send_cb(uv_udp_send_t* req, int status);
						static void m_uv_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

						std::shared_ptr<uv_udp_t> m_SocketHandle;
						bool m_isListenMode;
						bool m_isListening;
						bool m_hasInited;

						IpAddrV4 m_myRemoteAddr;
						IpAddrV4 m_myAddr;
					public:
						UDPAsyncSocketv4();
						UDPAsyncSocketv4(const IpAddrV4& ListeningAddr);
						UDPAsyncSocketv4(UDPAsyncSocketv4& oldUDP);

						void setmyIP(const IpAddrV4& myNewIP);
						void sendMsg(const IpAddrV4& Destination,const std::string& Data);
						void Listen();
						void Listen(const IpAddrV4& myNewAddr);
						void StopListen();

						void Init();
						void Destroy();

						virtual void onMsg(const IpAddrV4& SourceIP,const std::string& data);
						virtual void onError(int errCode, const std::string& errDescription);

						~UDPAsyncSocketv4();
				};
				class UDPAsyncSocketv6 {
					private:

					protected:
						static std::map<uv_udp_t*, std::vector<UDPAsyncSocketv6*>> m_MyClassPtrs;

						static void m_uv_read_cb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* remoteaddr, unsigned flags);
						static void m_uv_send_cb(uv_udp_send_t* req, int status);
						static void m_uv_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

						std::shared_ptr<uv_udp_t> m_SocketHandle;
						bool m_isListenMode;
						bool m_isListening;
						bool m_hasInited;

						IpAddrV6 m_myRemoteAddr;
						IpAddrV6 m_myAddr;
					public:
						UDPAsyncSocketv6();
						UDPAsyncSocketv6(const IpAddrV6& ListeningAddr);
						UDPAsyncSocketv6(UDPAsyncSocketv6& oldUDP);

						void setmyIP(const IpAddrV6& myNewIP);
						void sendMsg(const IpAddrV6& Destination, const std::string& Data);
						void Listen();
						void Listen(const IpAddrV6& myNewAddr);
						void StopListen();

						void Init();
						void Destroy();

						virtual void onMsg(const IpAddrV6& SourceIP, const std::string& data);
						virtual void onError(int errCode, const std::string& errDescription);

						~UDPAsyncSocketv6();
				};
			}
		}
	}
#endif