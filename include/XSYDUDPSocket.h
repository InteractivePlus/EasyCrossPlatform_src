#ifndef __EasyCrossPlatform_UDPSocketFile__
	#define __EasyCrossPlatform_UDPSocketFile__
	#include <EasyCP_Common.h>
	#include <libuv/uv.h>
	#include <XSYDMultiTask.h>
	#include <XSYDSocketResImpl.h>
	#include <XSYDStringUtil.h>
	namespace EasyCrossPlatform {
		namespace Network {
			namespace Socket {
				namespace UDP {
					class UDPAsyncClientAndServerSocket : public MailSenderSocket, public MailRecieverSocket {
					private:
						MailBoxRecieverMsgCallBack m_MsgCallBack = NULL;
						MailBoxRecieverErrorCallBack m_ErrCallBack = NULL;
					protected:
						void Init();
						void Destroy();

						static void m_uv_read_cb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* remoteaddr, unsigned flags);
						static void m_uv_send_cb(uv_udp_send_t* req, int status);
						static void m_uv_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

						uv_udp_t* m_SocketHandle = NULL;
						bool m_isListening = false;

						IpAddr m_myAddr;
						SocketWorker* mySocketWorker = NULL;

						void onError(int ErrNo, const std::string& ErrDescription);
						void onMsg(const IpAddr& mIP, const std::string& Data);
					public:
						UDPAsyncClientAndServerSocket(const IpAddr& ListeningAddr, SocketWorker* mWorker);
						UDPAsyncClientAndServerSocket(UDPAsyncClientAndServerSocket& oldUDP);

						void SetServerMsgCallBack(MailBoxRecieverMsgCallBack mCB);
						void SetServerErrCallBack(MailBoxRecieverErrorCallBack mCB);

						void SendMsg(const IpAddr& Destination, const std::string& Data);
						void SendMsg(const IpAddr& Destination, const std::vector<byte>& Data);
						void StartListen();
						void StopListen();
						bool isListening();

						void* CustomData;

						~UDPAsyncClientAndServerSocket();
					};
					class UDPAsyncClientSocket : public MailSenderSocket {
					private:
						
					protected:
						uv_udp_t * m_SocketHandle = NULL;
						static void m_uv_send_cb(uv_udp_send_t* req, int status);
						IpAddr m_myAddr;
						SocketWorker* mySocketWorker = NULL;
						void Init();
						void Destroy();
					public:
						UDPAsyncClientSocket(SocketWorker* mWorker, const IpAddr& BindAddr = IpAddr("::",0U));
						UDPAsyncClientSocket(UDPAsyncClientSocket& oldUDP);

						void SendMsg(const IpAddr& Destination, const std::string& Data);
						void SendMsg(const IpAddr& Destination, const std::vector<byte>& Data);

						void* CustomData;

						~UDPAsyncClientSocket();
					};
					class UDPAsyncServerSocket : public MailRecieverSocket {
					private:
						MailBoxRecieverMsgCallBack m_MsgCallBack = NULL;
						MailBoxRecieverErrorCallBack m_ErrCallBack = NULL;
					protected:
						uv_udp_t * m_SocketHandle = NULL;
						IpAddr m_myAddr;
						SocketWorker* mySocketWorker = NULL;
						void Init();
						void Destroy();
						bool m_isListening = false;
						void onError(int ErrNo, const std::string& ErrDescription);
						void onMsg(const IpAddr& mIP, const std::string& Data);

						static void m_uv_read_cb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* remoteaddr, unsigned flags);
						static void m_uv_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
					public:
						UDPAsyncServerSocket(const IpAddr& ListeningAddr, SocketWorker* mWorker);
						UDPAsyncServerSocket(UDPAsyncServerSocket& oldServer);
						void SetServerMsgCallBack(MailBoxRecieverMsgCallBack mCB);
						void SetServerErrCallBack(MailBoxRecieverErrorCallBack mCB);
						void StartListen();
						void StopListen();
						bool isListening();
						void* CustomData;
					};
				}
			}
		}
	}
#endif