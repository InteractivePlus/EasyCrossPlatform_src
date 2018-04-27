#ifndef __EasyCrossPlatform_TLSSocketFile__
	#define __EasyCrossPlatform_TLSSocketFile__
	#include <EasyCP_Common.h>
	#include <XSYDMultiTask.h>
	#include <libuv/uv.h>
	#include <XSYDSocketResImpl.h>
	#include <XSYDTCPSocket.h>
	#include <XSYDMultiTask.h>	
	#include <XSYDTrustedCert.h>
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
						mbedtls_entropy_context m_sslEntropy;
						mbedtls_ctr_drbg_context m_sslCtr_drbg;
						mbedtls_ssl_context m_sslContext;
						mbedtls_ssl_config m_sslConf;
						mbedtls_x509_crt m_sslCACert;
						bool m_Shaked = false;
						char** m_TMPALPNProtoList = NULL;
						unsigned int m_TMPALPNProtoNum = 0U;
						
						EasyCrossPlatform::Thread::SingleWork myWorkCls;

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
						bool CheckMsg();
					public:
						TLSClientConnectCallBack ConnectCallBack = NULL;
						TLSClientMsgCallBack MsgCallBack = NULL;
						TLSClientErrorCallBack ErrorCallBack = NULL;
						TLSClientDisconnectCallBack DisconnectCallBack = NULL;
						std::string serverHostName = "localhost";
						
						unsigned int MinHandshakeTime = 1000U;
						unsigned int MaxHandshakeTime = 60000U;

						std::vector<std::string> SupportedALPNProtocols;
						std::string NegotiatedALPNProtocols;
						bool VerifyServerCert = true;
						void setWorker(SocketWorker* newWorker);
						void setRemoteIPAddr(const IpAddr& newIP);
						void setSelfPort(const unsigned short Port);
						IpAddr getMyIpAddr();
						IpAddr getRemoteAddr();
						void setTrustedCAChain(const std::string& newCAChain);

						TLSAsyncClientSocket();
						TLSAsyncClientSocket(TLSAsyncClientSocket& oldClient);
						void* CustomData = NULL;
						void Init();
						void Connect();
						void Disconnect();
						void SendMsg(const std::string& Data);
						void SendMsg(const std::vector<byte>& Data);
						void Destroy();
						~TLSAsyncClientSocket();
				};
				class TLSSNIAsyncServerSingleConnection {
					friend class TLSSNIAsyncServer;
				private:

				protected:
					std::deque<byte> m_MsgWaitingForRead;
					bool m_Inited = false;
					mbedtls_entropy_context m_sslEntropy;
					mbedtls_ctr_drbg_context m_sslCtr_drbg;
					mbedtls_ssl_context m_sslContext;
					mbedtls_ssl_config m_sslConf;
					mbedtls_x509_crt m_sslDefaultCert;
					mbedtls_x509_crt m_sslCert;
					mbedtls_pk_context m_sslPrivateKey;
					mbedtls_pk_context m_sslDefaultPK;
					mbedtls_x509_crt m_sslCACert;

					char** m_TMPALPNProtoList = NULL;
					unsigned int m_TMPALPNProtoNum = 0U;
					bool m_Shaked = false;
					EasyCrossPlatform::Thread::SingleWork myWorkCls;

					static void TCPConnectCallBack(bool Succeed, void* ClientSocketPtr);
					static void TCPDisconnectCallBack(void* ClientSocketPtr);
					static void TCPMsgCallBack(const std::vector<byte>& Data, void* ClientSocketPtr);
					static void TCPErrorCallBack(int ErrNo, const std::string& ErrDescription, void* ClientSocketPtr);
					static void TLSDebugCallBack(void* TLSClientPtr, int level, const char* file, int line, const char* str);
					static int TLSRecvCallBack(void* TLSClientPtr, unsigned char* buf, size_t len);
					static int TLSSendCallback(void* TLSClientPtr, const unsigned char* buf, size_t len);
					static int TLSSNICallBack(void* MyClsPtr, mbedtls_ssl_context* ssl, const unsigned char* HostName, size_t HostNameLength);
					static void CompleteShakeProgress(std::thread::id myThreadID, void* Parameters, bool* RunningSign, std::mutex* Mutex);
					TCPAsyncClientSocket* m_ClientSocket = NULL;
					std::mutex ReadTCPMsgMutex;
					std::mutex SendTCPMsgMutex;
					void Init();
					void onDisconnected();
					void onConnected(bool Succeeded);
					void onMsgCB(const std::vector<byte>& Data);
					void onErrorOccured(int errNo, const std::string& ErrDescription);
					void onMbedTLSError(int mbedErrNo);
					void setWorker(SocketWorker* newWorker);
					void setSrvCert(const std::map < std::string,std::pair<std::string, std::pair<std::string,std::string>>> *newCerts); //first string is the hostname, default means all that does not match, first string in the pair is the cert content, second string in the pair is the private key.
					bool CheckNewMsg();
					const std::map<std::string, std::pair<std::string, std::pair<std::string,std::string>>>* m_SrvCerts;
				public:
					TLSClientConnectCallBack ConnectCallBack = NULL;
					TLSClientMsgCallBack MsgCallBack = NULL;
					TLSClientErrorCallBack ErrorCallBack = NULL;
					TLSClientDisconnectCallBack DisconnectCallBack = NULL;
					std::string serverHostName = "localhost";

					void* CustomData = NULL;

					bool VerifyServerCert = true;

					IpAddr getMyIpAddr();
					IpAddr getRemoteAddr();

					unsigned int MinHandshakeTime = 1000U;
					unsigned int MaxHandshakeTime = 60000U;

					std::vector<std::string> SupportedALPNProtocols;
					std::string NegotiatedALPNProtocols = "";

					TLSSNIAsyncServerSingleConnection();
					TLSSNIAsyncServerSingleConnection(TLSSNIAsyncServerSingleConnection& oldClient);
					void Disconnect();
					void SendMsg(const std::string& Data);
					void SendMsg(const std::vector<byte>& Data);
					void Destroy();
					void setCACert(const std::string& newCAChain);
					~TLSSNIAsyncServerSingleConnection();
				};
				class TLSSNIAsyncServer {
					private:

					protected:
						TCPAsyncServerSocket m_ServerSocket;
						static void TCPServerNewConnCallBack(void* newClientSocket, void* ServerClassPtr);
						static void TCPServerErrorCallBack(int errCode, const std::string& errInfo, void* ServerClassPtr);

						bool m_Inited = false;
						std::map<std::string, std::pair<std::string, std::pair<std::string,std::string>>> m_SrvCerts;

						void onError(int ErrNo, const std::string& ErrorDescription);
						void onMbedTLSError(int ErrNo);
						void onConnection(TLSSNIAsyncServerSingleConnection* NewClientPtr);
						std::string m_sslCACert;
					public:
						unsigned int MinHandshakeTime = 1000U;
						unsigned int MaxHandshakeTime = 60000U;
						TLSSNIAsyncServer();
						TLSSNIAsyncServer(TLSSNIAsyncServer& oldServer);
						std::vector<std::string> SupportedALPNProtocols;
						
						TLSServerNewConnectionCallBack ConnectionCallback = NULL;
						TLSServerErrorCallBack SrvErrorCallBack = NULL;
						TLSClientConnectCallBack ConnectCallBack = NULL;
						TLSClientMsgCallBack MsgCallBack = NULL;
						TLSClientErrorCallBack ErrorCallBack = NULL;
						TLSClientDisconnectCallBack DisconnectCallBack = NULL;
						void setIP(const IpAddr& myIP, int QueueLength = 0);
						void* CustomData = NULL;
						IpAddr getIP();
						void Listen();
						void StopListen();
						bool isListening();
						void Init();
						void Destroy();
						void setWorker(SocketWorker* newWorker);
						void setServerCerts(std::map<std::string, std::pair<std::string, std::pair<std::string,std::string>>>& newCerts); //first string is the hostname, default means all that does not match, first string in the pair is the cert content, second string in the pair is the private key.
						void setCACerts(const std::string& Cert);
				};
			}
		}
	}
#endif