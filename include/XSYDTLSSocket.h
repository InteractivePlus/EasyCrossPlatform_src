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
				class TLSAsyncClientSocket;
				class TLSSNIAsyncServerSingleConnection;
				class TLSSNIAsyncServer;

				class TLSAsyncClientSocket : public StandardClientSocket {
					private:
						StandardClientConnCallBack m_ConnectCallBack = NULL;
						StandardClientMsgCallBack m_MsgCallBack = NULL;
						StandardClientErrorCallBack m_ErrorCallBack = NULL;
						StandardClientDisconnectCallBack m_DisconnectCallBack = NULL;
						char** m_TMPALPNProtoList = NULL;
						unsigned int m_TMPALPNProtoNum = 0U;
					protected:
						std::string serverHostName = "localhost";

						unsigned int MinHandshakeTime = 1000U;
						unsigned int MaxHandshakeTime = 60000U;

						std::vector<std::string> SupportedALPNProtocols;
						bool VerifyServerCert = true;

						std::deque<byte> m_MsgWaitingForRead;
						mbedtls_entropy_context m_sslEntropy;
						mbedtls_ctr_drbg_context m_sslCtr_drbg;
						mbedtls_ssl_context m_sslContext;
						mbedtls_ssl_config m_sslConf;
						mbedtls_x509_crt m_sslCACert;
						
						bool m_Shaked = false;
						
						EasyCrossPlatform::Thread::SingleWork myWorkCls;

						static void TCPConnectCallBack(bool Succeed, StandardClientSocket* ClientSocketPtr);
						static void TCPDisconnectCallBack(StandardClientSocket* ClientSocketPtr);
						static void TCPMsgCallBack(const std::vector<byte>& Data, StandardClientSocket* ClientSocketPtr);
						static void TCPErrorCallBack(int ErrNo, const std::string& ErrDescription, StandardClientSocket* ClientSocketPtr);
						static void TLSDebugCallBack(void* TLSClientPtr, int level, const char* file, int line, const char* str);
						static int TLSRecvCallBack(void* TLSClientPtr, unsigned char* buf, size_t len);
						static int TLSSendCallback(void* TLSClientPtr, const unsigned char* buf, size_t len);
						static void CompleteShakeProgress(std::thread::id myThreadID, void* Parameters, bool* RunningSign, std::mutex* Mutex);
						TCPAsyncClientSocket* m_ClientSocket = NULL;
						std::mutex ReadTCPMsgMutex;
						std::mutex SendTCPMsgMutex;
						void onDisconnected();
						void onConnected(bool Succeeded);
						void onMsgCB(const std::vector<byte>& Data);
						void onErrorOccured(int errNo, const std::string& ErrDescription);
						void onMbedTLSError(int mbedErrNo);
						bool CheckMsg();

						void Init();
						void Destroy();
					public:
						TLSAsyncClientSocket(SocketWorker* mWorker, const IpAddr& RemoteAddr, bool IVerifyServerCert = true, const std::string& RemoteHostName = "localhost", const std::vector<std::string>& SupportALPNProtocols = std::vector<std::string>(), const unsigned int MinHandshakeTime = 1000U, const unsigned int MaxHandshakeTime = 60000U, unsigned short SelfPort = 0U);
						TLSAsyncClientSocket(TLSAsyncClientSocket& oldClient);
						
						std::string NegotiatedALPNProtocols;
						IpAddr getMyIpAddr();
						IpAddr getRemoteAddr();
						void setTrustedCAChain(const std::string& newCAChain);

						void SetConnectCallBack(StandardClientConnCallBack mCB);
						void SetMsgCallBack(StandardClientMsgCallBack mCB);
						void SetDisconnectCallBack(StandardClientDisconnectCallBack mCB);
						void SetErrorCallBack(StandardClientErrorCallBack mCB);

						void* CustomData = NULL;
						
						void Connect();
						void Disconnect();
						bool isConnected();
						void SendMsg(const std::string& Data);
						void SendMsg(const std::vector<byte>& Data);
						
						~TLSAsyncClientSocket();
				};
				class TLSSNIAsyncServerSingleConnection : public StandardClientSocket {
					friend class TLSSNIAsyncServer;
				private:
					StandardClientConnCallBack m_ConnectCallBack = NULL;
					StandardClientMsgCallBack m_MsgCallBack = NULL;
					StandardClientErrorCallBack m_ErrorCallBack = NULL;
					StandardClientDisconnectCallBack m_DisconnectCallBack = NULL;
				protected:
					TLSSNIAsyncServerSingleConnection(TCPAsyncClientSocket* mClient, const std::vector<std::string>& SupportALPNProtos, unsigned int MinHandshakeTime = 1000U, unsigned int MaxHandshakeTime = 60000U);
					unsigned int MinHandshakeTime = 1000U;
					unsigned int MaxHandshakeTime = 60000U;
					std::vector<std::string> SupportedALPNProtocols;

					std::deque<byte> m_MsgWaitingForRead;
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

					static void TCPConnectCallBack(bool Succeed, StandardClientSocket* ClientSocketPtr);
					static void TCPDisconnectCallBack(StandardClientSocket* ClientSocketPtr);
					static void TCPMsgCallBack(const std::vector<byte>& Data, StandardClientSocket* ClientSocketPtr);
					static void TCPErrorCallBack(int ErrNo, const std::string& ErrDescription, StandardClientSocket* ClientSocketPtr);
					static void TLSDebugCallBack(void* TLSClientPtr, int level, const char* file, int line, const char* str);
					static int TLSRecvCallBack(void* TLSClientPtr, unsigned char* buf, size_t len);
					static int TLSSendCallback(void* TLSClientPtr, const unsigned char* buf, size_t len);
					static int TLSSNICallBack(void* MyClsPtr, mbedtls_ssl_context* ssl, const unsigned char* HostName, size_t HostNameLength);
					static void CompleteShakeProgress(std::thread::id myThreadID, void* Parameters, bool* RunningSign, std::mutex* Mutex);
					TCPAsyncClientSocket* m_ClientSocket = NULL;
					std::mutex ReadTCPMsgMutex;
					std::mutex SendTCPMsgMutex;
					void Init();
					void Destroy();
					void onDisconnected();
					void onConnected(bool Succeeded);
					void onMsgCB(const std::vector<byte>& Data);
					void onErrorOccured(int errNo, const std::string& ErrDescription);
					void onMbedTLSError(int mbedErrNo);
					void setSrvCert(const std::map<std::string, std::pair<std::string, std::pair<std::string,std::string>>> *newCerts); //first string is the hostname, default means all that does not match, first string in the pair is the cert content, second string in the pair is the private key.
					bool CheckNewMsg();
					const std::map<std::string, std::pair<std::string, std::pair<std::string,std::string>>>* m_SrvCerts;
				public:
					TLSSNIAsyncServerSingleConnection(TLSSNIAsyncServerSingleConnection& oldClient);

					std::string serverHostName = "localhost";

					void* CustomData = NULL;

					IpAddr getMyIpAddr();
					IpAddr getRemoteAddr();

					std::string NegotiatedALPNProtocols = "";

					void SetConnectCallBack(StandardClientConnCallBack mCB);
					void SetMsgCallBack(StandardClientMsgCallBack mCB);
					void SetDisconnectCallBack(StandardClientDisconnectCallBack mCB);
					void SetErrorCallBack(StandardClientErrorCallBack mCB);

					void Connect();
					void Disconnect();
					void SendMsg(const std::string& Data);
					void SendMsg(const std::vector<byte>& Data);
					bool isConnected();
					
					~TLSSNIAsyncServerSingleConnection();
				};
				class TLSSNIAsyncServer : public StandardServerSocket {
					private:
						StandardServerNewConnectionCallBack m_ServerConnectionCallback = NULL;
						StandardServerErrorCallBack m_ServerErrorCallBack = NULL;
						StandardClientConnCallBack m_ClientConnectCallBack = NULL;
						StandardClientMsgCallBack m_ClientMsgCallBack = NULL;
						StandardClientErrorCallBack m_ClientErrorCallBack = NULL;
						StandardClientDisconnectCallBack m_ClientDisconnectCallBack = NULL;
					protected:
						

						unsigned int MinHandshakeTime = 1000U;
						unsigned int MaxHandshakeTime = 60000U;
						std::vector<std::string> SupportedALPNProtocols;

						void Init();
						void Destroy();
						TCPAsyncServerSocket* m_ServerSocket = NULL;
						static void TCPServerNewConnCallBack(StandardClientSocket* newClientSocket, StandardServerSocket* ServerClassPtr);
						static void TCPServerErrorCallBack(int errCode, const std::string& errInfo, StandardServerSocket* ServerClassPtr);

						bool m_Inited = false;
						std::map<std::string, std::pair<std::string, std::pair<std::string,std::string>>> m_SrvCerts; //first string is the hostname, default means all that does not match, first string in the pair is the cert content, second string in the pair is the private key.

						void onError(int ErrNo, const std::string& ErrorDescription);
						void onMbedTLSError(int ErrNo);
						void onConnection(TLSSNIAsyncServerSingleConnection* NewClientPtr);
					public:
						TLSSNIAsyncServer(const IpAddr& BindIP, SocketWorker* mWorker, const std::map<std::string,std::pair<std::string,std::pair<std::string,std::string>>>& ServerCerts, int QueLength = 500);
						TLSSNIAsyncServer(TLSSNIAsyncServer& oldServer);
						
						
						void* CustomData = NULL;
						IpAddr getIP();
						void StartListen();
						void StopListen();
						bool isListening();
				};
			}
		}
	}
#endif