/* 
Original Author - @itomi on github
github address https://github.com/itomi/CppSocket
Modified and Bug Fixed by Windy
*/
#ifndef __XSYDFileTCPSockFile__
#define __XSYDFileTCPSockFile__

#include "CrossPlatform_Socket.h"
#include "EasyCP_Common.h"
namespace EasyCrossPlatform {
	namespace Network {
		class TCPSocket : public Socket
		{
		public:
			enum TCPSocketMode
			{
				Server,
				Client,
				NotSet
			};

		private:
			bool               m_Connected;
			unsigned short    m_PortNumber;
			TCPSocketMode       m_TCPMode;

		public:
			TCPSocket();
			TCPSocket(TCPSocket& TCPSocketObject);
			TCPSocket(Socket& SocketObject);
			TCPSocket(unsigned short PortNumber);
			TCPSocket(const char* IPAddress, unsigned short PortNumber);
			TCPSocket(std::string& IPAddress, unsigned short PortNumber);

			virtual ~TCPSocket();

			virtual int         Connect(const char* IPAddress, unsigned short PortNumber);
			virtual int         Connect(std::string& IPAddress, unsigned short PortNumber);
			virtual int         Disconnect();

			virtual TCPSocket*  Accept();

			virtual int         Send(const void* Buffer, int Size);
			virtual int         Receive(const void* Buffer, int Size, bool BlockMode = false);
			virtual const bool isConnected();
			virtual bool        Close();
		};
	}
}
#endif //__XSYDFileTCPSockFile__
