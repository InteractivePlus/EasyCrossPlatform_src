#ifndef __XSYDFileUDPSockFile__
#define __XSYDFileUDPSockFile__

#include "CrossPlatform_Socket.h"
#include "EasyCP_Common.h"
namespace EasyCrossPlatform {
	namespace Network {
		class UDPSocket : public Socket
		{
		protected:
			int                 m_Domain;
			const int           m_Type;
			const int           m_Protocol;
			struct sockaddr_in  m_ReadSocketAddress;
			struct sockaddr_in  m_SendSocketAddress;
			#ifdef EASYCROSSPLATFORM_PLATFORM_LINUX
			socklen_t           m_Structuresize;
			#else
			int                 m_Structuresize;
			#endif

			bool                m_SendAddressIsSet;

		public:
			UDPSocket();
			virtual ~UDPSocket();

			virtual bool    Listen(int PortNumber);
			virtual void    SetAddress(const char* IPAddress, unsigned short PortNumber);
			virtual int     Read(void* Buffer, int BufferSize);
			virtual int    Send(void* Buffer, int BufferSize);
			virtual bool    Close();
		};
	}
}

#endif //__XSYDFileUDPSockFile__
