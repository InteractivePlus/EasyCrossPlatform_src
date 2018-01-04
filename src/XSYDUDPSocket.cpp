#include <XSYDUDPSocket.h>

EasyCrossPlatform::Network::UDPSocket::UDPSocket()
	: Socket(Socket::UDP,"UDP"),
	m_Domain(Socket::IPv4),
	m_Type(Socket::UDP),
	m_Protocol(0),
	m_SendAddressIsSet(false)
{
	if ((this->m_SocketDescriptor = socket(m_Domain, m_Type, m_Protocol)) == -1)
	{
		this->ProvideErrorString();
		throw SocketException(m_ErrorString);
	}
	else
	{
		this->m_ErrorFlag = 0;
	}
}

EasyCrossPlatform::Network::UDPSocket::~UDPSocket()
{
	this->Close();
}

bool EasyCrossPlatform::Network::UDPSocket::Listen(int PortNumber)
{
	struct sockaddr_in TemporarySocketAddress;
	memset(&TemporarySocketAddress, 0, sizeof(TemporarySocketAddress));
	if (this->m_ErrorFlag == 0)
	{
		TemporarySocketAddress.sin_family = AF_INET;
		TemporarySocketAddress.sin_port = htons(PortNumber);
		TemporarySocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		
		if (bind(this->m_SocketDescriptor, (struct sockaddr*) &TemporarySocketAddress, sizeof(struct sockaddr_in)) == -1)
		{
			ProvideErrorString();
			throw SocketException(m_ErrorString);
			return false;
		}
		return true;
	}
	return false;
}

void EasyCrossPlatform::Network::UDPSocket::SetAddress(const char* IPAddress, unsigned short PortNumber)
{
	memset(&m_ReadSocketAddress, 0, sizeof(struct sockaddr_in));
	m_ReadSocketAddress.sin_family = Socket::IPv4;
	m_ReadSocketAddress.sin_port = htons(PortNumber);

	if (IPAddress == NULL)
	{
		m_ReadSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else {
		m_ReadSocketAddress.sin_addr.s_addr = inet_addr(IPAddress);
	}
	this->m_Structuresize = sizeof(struct sockaddr_in);
}

int EasyCrossPlatform::Network::UDPSocket::Read(void* Buffer, int BufferSize)
{
	if (this->m_SocketDescriptor == -1)
	{
		throw SocketException("UDPSocket is not created.");
		return -1;
	}

	return recvfrom(this->m_SocketDescriptor, (char*)Buffer, BufferSize, 0, (struct sockaddr*)&(this->m_ReadSocketAddress), &(this->m_Structuresize));
}

int EasyCrossPlatform::Network::UDPSocket::Send(void* Buffer, int BufferSize)
{
	if (this->m_SocketDescriptor == -1)
	{
		throw SocketException("UDPSocket is not created.");
		return -1;
	}

	if (!m_SendAddressIsSet)
	{
		throw SocketException("Send address is not set.");
		return -1;
	}
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
	int ReturnValue = sendto(this->m_SocketDescriptor, (char*)Buffer, BufferSize, 0, (struct sockaddr*)&m_SendSocketAddress, sizeof(struct sockaddr));
#else
	int ReturnValue = sendto(this->m_SocketDescriptor, Buffer, BufferSize, 0, (struct sockaddr*)&m_SendSocketAddress, sizeof(struct sockaddr));
#endif
	if (ReturnValue == -1)
	{
		this->ProvideErrorString();
		throw SocketException(m_ErrorString);
	}

	return ReturnValue;
}

bool EasyCrossPlatform::Network::UDPSocket::Close()
{
	return this->Socket::Close();
}
