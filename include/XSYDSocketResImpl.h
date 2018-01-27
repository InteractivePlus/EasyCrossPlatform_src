#ifndef __EasyCrossPlatform_SocketResImplFile__
	#define __EasyCrossPlatform_SocketResImplFile__
	#include <EasyCP_Common.h>
	#include <libuv/uv.h>
	#include <XSYDMultiTask.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Socket {
				//IpV4地址类封装, 绑定全部网卡填0.0.0.0
				class IpAddrV4 {
					friend class TCPAsyncClientSocketv4;
					friend class UDPAsyncSocketv4;
				private:

				protected:
					sockaddr_in m_Addr;
				public:
					IpAddrV4();
					IpAddrV4(const std::string& IpAddress, const unsigned short Port);
					IpAddrV4(const char *IpAddress, const unsigned short Port);
					bool setIPAddress(const std::string& IpAddress, const unsigned short Port);
					bool setIPAddress(const char * IpAddress, const unsigned short Port);
					bool setIPAddress(const sockaddr_in& newAddr);
					std::string getIPString();
					unsigned short getPort();
					sockaddr_in getIPAddress();
				};
				class IpAddrV6 {
					friend class TCPAsyncClientSocketv6;
					friend class UDPAsyncSocketv6;
				private:

				protected:
					sockaddr_in6 m_Addr;
				public:
					IpAddrV6();
					IpAddrV6(const std::string& IpAddress, const unsigned short Port);
					IpAddrV6(const char *IpAddress, const unsigned short Port);
					bool setIPAddress(const std::string& IpAddress, const unsigned short Port);
					bool setIPAddress(const char * IpAddress, const unsigned short Port);
					bool setIPAddress(const sockaddr_in6& newAddr);
					std::string getIPString();
					unsigned short getPort();
					sockaddr_in6 getIPAddress();
				};
				class SocketParam {
					private:

					protected:
						
					public:
						static std::mutex Lock;
						static unsigned int m_num_Client;
						static uv_loop_t m_uv_loop;
						static EasyCrossPlatform::Thread::SingleWork m_MTManager;
						static void m_MultiThread_Job(std::thread::id ThreadID, void* Parameters, bool * RunningSign, std::mutex *Mutex);
				};
			}
		}
	}
#endif