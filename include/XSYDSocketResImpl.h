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
					friend class UDPAsyncClientAndServerSocket;
					friend class TCPAsyncClientSocket;
					friend class TCPAsyncServerSocket;
					friend class DNSRequest;
					protected:
						sockaddr m_Addr;
						bool m_isIpV4 = true;
					public:
						IpAddr();
						IpAddr(const std::string& IpAddress, const unsigned short Port);
						IpAddr(const char* IpAddress, const unsigned short Port);
						IpAddr(const sockaddr& newAddr);
						IpAddr(const IpAddr& oldAddr);
						bool setIPAddress(const std::string& IpAddress, const unsigned short Port);
						bool setIPAddress(const char* IpAddress, const unsigned short Port);
						bool setIPAddress(const sockaddr& newAddr);
						std::string getIPString();
						bool addrIsIPV4();
						unsigned short getPort();
						sockaddr getIPAddress();
				};
				class SocketParam {
					private:

					protected:
						static EasyCrossPlatform::Thread::SingleWork m_MTManager;
					public:
						static unsigned int m_num_Client;
						static uv_loop_t m_uv_loop;
						static void Start();
						static void Stop();
						
						static void m_MultiThread_Job(std::thread::id ThreadID, void* Parameters, bool * RunningSign, std::mutex *Mutex);
				};
				class SocketWorker {
					private:
						
					protected:
						void Start();
						void Stop();
					public:
						SocketWorker();
						SocketWorker(SocketWorker& LeftSocket);
						unsigned int m_num_Client = 0U;
						std::shared_ptr<uv_loop_t> m_uv_loop;
						EasyCrossPlatform::Thread::SingleWork m_MTManager;
						void Increment();
						void Decrement();
						static void m_MultiThread_Job(std::thread::id ThreadID, void* Parameters, bool * RunningSign, std::mutex *Mutex);
						~SocketWorker();
				};
				
				class StandardClientSocket;
				typedef void(*StandardClientConnCallBack)(bool, StandardClientSocket*);
				typedef void(*StandardClientMsgCallBack)(const std::vector<byte>&, StandardClientSocket*);
				typedef void(*StandardClientDisconnectCallBack)(StandardClientSocket*);
				typedef void(*StandardClientErrorCallBack)(int ErrNo, const std::string& ErrDescription, StandardClientSocket*);

				class StandardServerSocket;
				typedef void(*StandardServerNewConnectionCallBack)(StandardClientSocket*, StandardServerSocket*);
				typedef void(*StandardServerErrorCallBack)(int ErrNo, const std::string& ErrDescription, StandardServerSocket*);
				class StandardClientSocket {
				public:
					virtual void SetConnectCallBack(StandardClientConnCallBack) = 0;
					virtual void SetMsgCallBack(StandardClientMsgCallBack) = 0;
					virtual void SetDisconnectCallBack(StandardClientDisconnectCallBack) = 0;
					virtual void SetErrorCallBack(StandardClientErrorCallBack) = 0;
					virtual void SendMsg(const std::string&) = 0;
					virtual void SendMsg(const std::vector<byte>&) = 0;
					virtual void Connect() = 0;
					virtual void Disconnect() = 0;
					virtual bool isConnected() = 0;
				};
				class StandardServerSocket {
				public:
					virtual void SetClientConnectCallBack(StandardClientConnCallBack) = 0;
					virtual void SetClientMsgCallBack(StandardClientMsgCallBack) = 0;
					virtual void SetClientDisconnectCallBack(StandardClientDisconnectCallBack) = 0;
					virtual void SetClientErrorCallBack(StandardClientErrorCallBack) = 0;
					virtual void SetServerNewConnCallBack(StandardServerNewConnectionCallBack) = 0;
					virtual void SetServerErrorCallBack(StandardServerErrorCallBack) = 0;
					virtual void StartListen() = 0;
					virtual void StopListen() = 0;
					virtual bool isListening() = 0;
				};

				class MailSenderSocket;

				class MailRecieverSocket;
				typedef void(*MailBoxRecieverMsgCallBack)(const std::vector<byte>&, const IpAddr& IncommingIP, MailRecieverSocket*);
				typedef void(*MailBoxRecieverErrorCallBack)(int, const std::string&, MailRecieverSocket*);
				class MailSenderSocket {
					virtual void SendMsg(const IpAddr&, const std::string&) = 0;
					virtual void SendMsg(const IpAddr&, const std::vector<byte>&) = 0;
				};

				
				class MailRecieverSocket {
					virtual void SetServerMsgCallBack(MailBoxRecieverMsgCallBack) = 0;
					virtual void SetServerErrCallBack(MailBoxRecieverErrorCallBack) = 0;
					virtual void StartListen() = 0;
					virtual void StopListen() = 0;
					virtual bool isListening() = 0;
				};
			}
		}
	}
#endif