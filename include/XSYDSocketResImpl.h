#ifndef __EasyCrossPlatform_SocketResImplFile__
	#define __EasyCrossPlatform_SocketResImplFile__
	#include <EasyCP_Common.h>
	#include <libuv/uv.h>
	#include <XSYDMultiTask.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Socket {
				//IpV4绑定全部网卡填0.0.0.0
				//IpV6绑定全部网卡填::
				class IpAddr {
					friend class UDPAsyncSocket;
					friend class TCPAsyncClientSocket;
					friend class TCPAsyncServerSocket;
					friend class DNSRequest;
					protected:
						sockaddr m_Addr;
						bool m_isIpV4;
					public:
						IpAddr();
						IpAddr(const std::string& IpAddress, const unsigned short Port, bool AddrIpV4);
						IpAddr(const char* IpAddress, const unsigned short Port, bool AddrIpV4);
						IpAddr(const sockaddr& newAddr);
						IpAddr(const IpAddr& oldAddr);
						bool setIPAddress(const std::string& IpAddress, const unsigned short Port, bool AddrIpV4);
						bool setIPAddress(const char* IpAddress, const unsigned short Port, bool AddrIpV4);
						bool setIPAddress(const sockaddr& newAddr);
						std::string getIPString();
						bool addrIsIPV4();
						unsigned short getPort();
						sockaddr getIPAddress();
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