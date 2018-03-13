#ifndef __EasyCrossPlatform_TLSSocketFile__
	#define __EasyCrossPlatform_TLSSocketFile__
	#include <EasyCP_Common.h>
	#include <XSYDMultiTask.h>
	#include <libuv/uv.h>
	#include <XSYDSocketResImpl.h>
	#include <XSYDTCPSocket.h>
	#include <XSYDMultiTask.h>	

	#include <mbedtls/net.h>
	#include <mbedtls/ssl.h>
	#include <mbedtls/entropy.h>
	#include <mbedtls/ctr_drbg.h>
	#include <mbedtls/debug.h>
	#include <mbedtls/error.h>

	namespace EasyCrossPlatform {
		namespace Network {
			namespace Socket {
				const constexpr int MBEDTLS_DEBUGLEVEL = 0;
				const constexpr char* defaultAuthorityCA = "";
				typedef void(*TLSClientMsgCallBack)(const std::vector<byte>&, void*);
				typedef void(*TLSClientConnectCallBack)(bool, void*);
				typedef void(*TLSClientDisconnectCallBack)(void*);
				typedef void(*TLSServerNewConnectionCallBack)(void*, void*);
				typedef void(*TLSClientErrorCallBack)(int, const std::string&, void*);
				typedef void(*TLSServerErrorCallBack)(int, const std::string&, void*);
				class TLSAsyncClientSocket {
					private:

					protected:
						std::deque<byte> m_MsgWaitingForRead;
						bool m_Inited = false;
						static std::map<TCPAsyncClientSocket*, TLSAsyncClientSocket*> MyClassPtrs;
						mbedtls_entropy_context m_sslEntropy;
						mbedtls_ctr_drbg_context m_sslCtr_drbg;
						mbedtls_ssl_context m_sslContext;
						mbedtls_ssl_config m_sslConf;
						mbedtls_x509_crt m_sslCACert;
						bool m_Shaked = false;

						static void TCPConnectCallBack(bool Succeed, void* ClientSocketPtr);
						static void TCPDisconnectCallBack(void* ClientSocketPtr);
						static void TCPMsgCallBack(const std::vector<byte>& Data, void* ClientSocketPtr);
						static void TCPErrorCallBack(int ErrNo, const std::string& ErrDescription, void* ClientSocketPtr);
						static void TLSDebugCallBack(void* TLSClientPtr, int level, const char* file, int line, const char* str);
						static int TLSRecvCallBack(void* TLSClientPtr, unsigned char* buf, size_t len);
						static int TLSSendCallback(void* TLSClientPtr, const unsigned char* buf, size_t len);
						static void CompleteShakeProgress(std::thread::id myThreadID, void* Parameters, bool* RunningSign, std::mutex* Mutex);
						TCPAsyncClientSocket m_ClientSocket;
						std::mutex ReadTCPMsgMutex;
						std::mutex SendTCPMsgMutex;
						void onDisconnected();
						void onConnected(bool Succeeded);
						void onMsgCB(const std::vector<byte>& Data);
						void onErrorOccured(int errNo, const std::string& ErrDescription);
						void onMbedTLSError(int mbedErrNo);
					public:
						TLSClientConnectCallBack ConnectCallBack = NULL;
						TLSClientMsgCallBack MsgCallBack = NULL;
						TLSClientErrorCallBack ErrorCallBack = NULL;
						TLSClientDisconnectCallBack DisconnectCallBack = NULL;
						std::string serverHostName = "localhost";
						EasyCrossPlatform::Thread::SingleWork myWorkCls;

						bool VerifyServerCert = true;
						void setWorker(SocketWorker* newWorker);
						void setRemoteIPAddr(const IpAddr& newIP);
						IpAddr getMyIpAddr();
						IpAddr getRemoteAddr();
						void setTrustedCAChain(const std::string& newCAChain);

						TLSAsyncClientSocket();
						TLSAsyncClientSocket(TLSAsyncClientSocket& oldClient);
						void Init();
						void Connect();
						void Disconnect();
						void SendMsg(const std::string& Data);
						void SendMsg(const std::vector<byte>& Data);
						void Destroy();
						~TLSAsyncClientSocket();
				};
			}
		}
	}
#endif