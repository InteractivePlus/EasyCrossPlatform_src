#ifndef __EasyCrossPlatform_TCPAsyncServerFile__
	#define __EasyCrossPlatform_TCPAsyncServerFile__
	#include <EasyCP_Common.h>
	#include <CrossPlatform_Socket.h>
	#include <XSYDTCPSocket.h>
	#include <XSYDMultiTask.h>

	namespace EasyCrossPlatform {
		namespace Network {
			#define TCPAsyncServerTmpSize 1024
			class TCPAsyncSocket {
			private:
				unsigned short m_PortNum = 0;
				bool isListening = false;
				bool m_hasBind = false;
				TCPSocket m_Socket;
				std::vector<TCPSocket*> m_ClientSockets;
				std::mutex* m_ConnectionMutex;
				std::mutex* m_MsgMutex;
				std::mutex m_ClientSocketListMutex;
				EasyCrossPlatform::Thread::SingleWork* m_ConnectionThread;
				EasyCrossPlatform::Thread::SingleWork* m_MsgThread;
				void StartUp(bool UseSameMutex = true);
				static void SuperviseConnectionThread(std::thread::id ThreadID, void* ClassObj, bool * RunningSign, std::mutex *Mutex);
				static void SuperviseMsgThread(std::thread::id ThreadID, void* ClassObj, bool * RunningSign, std::mutex *Mutex);
			protected:

			public:
				//if UseSameMutex was set to true, then the callback of Connected and Msg will share the same thread locker
				TCPAsyncSocket(bool UseSameMutex = true);
				TCPAsyncSocket(unsigned short PortNumber, bool UseSameMutex = true);
				TCPAsyncSocket(TCPAsyncSocket& RightObject);
				~TCPAsyncSocket();
				bool Listen();
				bool Listen(unsigned short PortNumber);
				bool StopListen();
				virtual void ConnectedCallBack(TCPSocket &MySocket, std::mutex &ConnectionMutex);
				virtual void MsgCallBack(TCPSocket &MySocket, std::string Msg ,std::mutex &MsgMutex);
				virtual void DisconnectCallBack(TCPSocket &MySocket, std::mutex &ConnectionMutex);
			};
		}
	}
#endif
