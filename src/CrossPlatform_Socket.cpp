#include "CrossPlatform_Socket.h"
using namespace std;
void Socket::ProvideErrorString()
{
#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
	m_ErrorFlag = errno;
	m_ErrorString = std::string(strerror(m_ErrorFlag));
#else
	char buffer[129];
	this->m_ErrorFlag = WSAGetLastError();
	_strerror_s(buffer, 128, "");
	this->m_ErrorString = std::string(buffer, 128);
#endif

}

bool Socket::SocketInit()
{
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
	WSADATA wsdat;
	memset(&wsdat, 0, sizeof(wsdat));

	if (WSAStartup(MAKEWORD(2, 2), &wsdat))
		return false;
#endif
	return true;
}

Socket::Socket()
	: m_SocketDescriptor(0),
	m_SocketProtocol(0),
	m_Domain(IPv4),
	m_Mode(TCP),
	m_ConnectionQueueSize(SOMAXCONN),
	m_IPAddress(0),
	m_PortNumber(0),
	m_ErrorFlag(0)
{
	if (this->SocketInit() == false)
	{
		ProvideErrorString();
		throw SocketException(m_ErrorString);
	}
}

Socket::Socket(Socket& SocketObj)
	: m_SocketDescriptor(SocketObj.m_SocketDescriptor),
	m_SocketProtocol(SocketObj.m_SocketProtocol),
	m_Domain(SocketObj.m_Domain),
	m_Mode(SocketObj.m_Mode),
	m_ConnectionQueueSize(SocketObj.m_ConnectionQueueSize),
	m_IPAddress(SocketObj.m_IPAddress),
	m_PortNumber(SocketObj.m_PortNumber),
	m_ErrorFlag(SocketObj.m_ErrorFlag)
{
	if (this->SocketInit() == false)
	{
		ProvideErrorString();
		throw SocketException(m_ErrorString);
	}
}

Socket::Socket(SocketMode Mode, SocketProtocol Protocol, int Domain, unsigned short MaxConnection)
	: m_SocketDescriptor(0),
	m_SocketProtocol(Protocol),
	m_Domain(Domain),
	m_Mode(Mode),
	m_ConnectionQueueSize(MaxConnection),
	m_IPAddress(0),
	m_PortNumber(0),
	m_ErrorFlag(0)
{
	if (this->SocketInit() == false)
	{
		ProvideErrorString();
		throw SocketException(m_ErrorString);
	}
}
Socket::Socket(SocketMode Mode, const char* Protocol, int Domain, unsigned short MaxConnection)
	: m_SocketDescriptor(0),
	m_Domain(Domain),
	m_Mode(Mode),
	m_ConnectionQueueSize(MaxConnection),
	m_IPAddress(0),
	m_PortNumber(0),
	m_ErrorFlag(0)
{
	if (this->SocketInit() == false) {
		ProvideErrorString();
		throw SocketException(m_ErrorString);
	}
	this->m_SocketProtocol = Socket::GetSocketProtocol(Protocol);
}


Socket::~Socket()
{
	#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
	    WSACleanup();
	#endif // _WIN32
}

int Socket::SetProtocol(SocketProtocol Protocol)
{
	if (this->m_SocketDescriptor == 0)
	{
		this->m_SocketProtocol = Protocol;
		return Protocol;
	}
	return -1;
}

int Socket::SetProtocol(const char* ProtocolName)
{
	return (this->SetProtocol(Socket::GetSocketProtocol(ProtocolName)));
}

int Socket::SetProtocol(std::string ProtocolName)
{
	return (this->SetProtocol(ProtocolName.c_str()));
}

int Socket::SetDomain(int Domain)
{
	if (this->m_SocketDescriptor == 0)
	{
		this->m_Domain = Domain;
		return Domain;
	}
	return -1;
}

#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
int
#else
SOCKET
#endif
Socket::Create()
{
	if (this->m_SocketDescriptor == 0)
	{
		if ((this->m_SocketDescriptor = socket(this->m_Domain, this->m_Mode, this->m_SocketProtocol)) == -1)
		{
			ProvideErrorString();
			this->m_SocketDescriptor = 0;
			throw SocketException(m_ErrorString);
		}
		return this->m_SocketDescriptor;
	}

	throw SocketException("Socket is already created.");
#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
	return -1;
#else
	return 0;
#endif
}
int Socket::Bind(unsigned short PortNumber)
{
	return (this->Bind(PortNumber, "0.0.0.0"));
}

int Socket::Bind(unsigned short PortNumber, const char* IPAddress)
{
	struct sockaddr_in  TemporaryBindAddress_IPv4;
	struct sockaddr_in6 TemporaryBindAddress_IPv6;
	if (this->m_SocketDescriptor > 0)
	{
		if (this->m_Domain == IPv4)
		{
			memset(&TemporaryBindAddress_IPv4, 0, sizeof(struct sockaddr_in));

			TemporaryBindAddress_IPv4.sin_family = IPv4;
			TemporaryBindAddress_IPv4.sin_port = htons(PortNumber);
			TemporaryBindAddress_IPv4.sin_addr.s_addr = inet_addr(IPAddress);

			if ((bind(this->m_SocketDescriptor, (struct sockaddr*)&TemporaryBindAddress_IPv4, sizeof(struct sockaddr))) == -1)
			{
				ProvideErrorString();
				throw SocketException(m_ErrorString);
				return -1;
			}
			this->m_PortNumber = PortNumber;
		}
		else if (this->m_Domain == IPv6)
		{
			memset(&TemporaryBindAddress_IPv6, 0, sizeof(struct sockaddr_in6));

			TemporaryBindAddress_IPv6.sin6_family = IPv6;
			TemporaryBindAddress_IPv6.sin6_port = htons(PortNumber);

			inet_pton(this->m_Domain, IPAddress, &(TemporaryBindAddress_IPv6.sin6_addr));

			if ((bind(this->m_SocketDescriptor, (struct sockaddr*)&TemporaryBindAddress_IPv6, sizeof(struct sockaddr))) == -1)
			{
				ProvideErrorString();
				throw SocketException(m_ErrorString);
				return -1;
			}
			this->m_PortNumber = PortNumber;
		}

		return 0;
	}

	throw SocketException("Socket is not created.");
	return -1;
}

int Socket::Bind(unsigned short PortNumber, std::string& IPAddress)
{
	return (this->Bind(PortNumber, IPAddress.c_str()));
}

int Socket::Listen()
{
	if (this->m_SocketDescriptor > 0)
	{
		if (listen(this->m_SocketDescriptor, this->m_ConnectionQueueSize) == -1)
		{
			ProvideErrorString();
			throw SocketException(m_ErrorString);
			return -1;
		}
		return 0;
	}
	throw SocketException("Socket is not created.");
	return -1;
}

int Socket::Listen(unsigned short PortNumber)
{
	if (this->Bind(PortNumber) == 0)
		return (this->Listen());

	throw SocketException("Unexpected error: Socket::Listen(PortNumber)");
	return -1;
}

int Socket::Connect(const char* IPAddress, unsigned short PortNumber)
{
	int ReturnValue = 0;

	if (this->m_SocketDescriptor == 0)
	{
		throw SocketException("Socket is not created.");
		return -1;
	}
	else
	{
		if (this->m_Domain == IPv4)
		{
			struct sockaddr_in  TemporaryAddressStructure_IPv4;
			memset(&TemporaryAddressStructure_IPv4, 0, sizeof(struct sockaddr_in));

			TemporaryAddressStructure_IPv4.sin_family = m_Domain;
			TemporaryAddressStructure_IPv4.sin_port = htons(PortNumber);
			TemporaryAddressStructure_IPv4.sin_addr.s_addr = inet_addr(IPAddress);

			memset(&TemporaryAddressStructure_IPv4.sin_zero, 0, sizeof(TemporaryAddressStructure_IPv4.sin_zero));

			if ((ReturnValue = connect(this->m_SocketDescriptor, (struct sockaddr*)&TemporaryAddressStructure_IPv4, sizeof(struct sockaddr))) != 0)
			{
				ProvideErrorString();
				throw SocketException(m_ErrorString);
				return -1;
			}

			return ReturnValue;

		}
		else if (this->m_Domain == IPv6)
		{
			struct sockaddr_in6 TemporaryAddressStructure_IPv6;

			memset(&TemporaryAddressStructure_IPv6, 0, sizeof(struct sockaddr_in6));

			TemporaryAddressStructure_IPv6.sin6_family = m_Domain;
			TemporaryAddressStructure_IPv6.sin6_port = htons(PortNumber);
			inet_pton(this->m_Domain, IPAddress, &(TemporaryAddressStructure_IPv6.sin6_addr));

			if ((ReturnValue = connect(this->m_SocketDescriptor, (struct sockaddr*)&TemporaryAddressStructure_IPv6, sizeof(struct sockaddr))) != 0)
			{
				ProvideErrorString();
				throw SocketException(m_ErrorString);
				return -1;
			}

			return ReturnValue;

		}
		else
		{
			throw SocketException("Domain unset.");
			return -1;
		}
	}
}

int Socket::Connect(std::string& IPAddress, unsigned short PortNumber)
{
	return (this->Connect(IPAddress.c_str(), PortNumber));
}

Socket* Socket::Accept()
{
	if (this->m_SocketDescriptor == 0)
	{
		throw SocketException("Socket is not created. Accept.");
		return NULL;
	}

	if (this->m_Mode == Socket::UDP)
	{
		throw SocketException("Socket is set for UDP communication.");
		return NULL;
	}
	struct sockaddr_in Remote;

#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
	SOCKET RemoteSocket;
	int StructSize;
#else
	socklen_t StructSize;
	int RemoteSocket;
#endif

	StructSize = sizeof(sockaddr_in);
	RemoteSocket = accept(this->m_SocketDescriptor, (struct sockaddr*)&Remote, &StructSize);
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
	if(RemoteSocket == INVALID_SOCKET)
#else
	if (RemoteSocket == -1)
#endif
	{
		ProvideErrorString();
		int ErrorNum = this->GetError();
		if (ErrorNum != 0 && ErrorNum != EAGAIN && ErrorNum != EWOULDBLOCK) {
			throw SocketException(m_ErrorString);
		}
		return NULL;
	}

	Socket* SocketObjectToReturn = new Socket(Socket::TCP, "TCP", Socket::IPv4, SOMAXCONN);

	SocketObjectToReturn->m_SocketDescriptor = RemoteSocket;
	SocketObjectToReturn->m_PortNumber = htons(Remote.sin_port);
	memcpy(&SocketObjectToReturn->m_IPAddress, &Remote.sin_addr.s_addr, 4);


	return SocketObjectToReturn;
}

int Socket::Read(const void* Buffer, int Size, bool Block)
{
	if (this->m_SocketDescriptor == 0)
	{
		throw SocketException("Socket is not created.");
		return -1;
	}

	if (this->m_Mode == Socket::UDP)
	{
		throw SocketException("Socket is set to UDP, not TCP.");
		return -1;
	}
	memset((void*)Buffer, 0, Size);
	int ReturnValue = 0;
#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
	int Linux_Recv_flag = 0;
	if (!Block) {
		Linux_Recv_flag = MSG_DONTWAIT;
	}
	ReturnValue = recv(this->m_SocketDescriptor, (void*)Buffer, Size, Linux_Recv_flag); //Linux MSG_DONTWAIT表示非阻塞
#else
	u_long mode = 1;
	if (!Block) {
		//设置为非阻塞模式
		ioctlsocket(this->m_SocketDescriptor, FIONBIO, &mode);
	}
	ReturnValue = recv(this->m_SocketDescriptor, (char*)Buffer, Size, 0);
#endif
	if (ReturnValue == 0)
	{
		//throw SocketException("Server shutting down.");
		return 0;
	}
	else if (ReturnValue == -1) {
		ProvideErrorString();
		int ErrCode = 0;
		ErrCode = this->GetError();
		
		if (ErrCode != 0 && ErrCode != EAGAIN && ErrCode != EWOULDBLOCK)
		{
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
			if (ErrCode != WSAEWOULDBLOCK) {
				//throw SocketException(m_ErrorString);
				return 0;
			}
#else
			//throw SocketException(m_ErrorString);
			return 0;
#endif
		}
	}
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
	if (!Block) {
		mode = 0; //重新设置为阻塞模式
		ioctlsocket(this->m_SocketDescriptor, FIONBIO, &mode);
	}
#endif

	return ReturnValue;
}

int Socket::Write(const void* Buffer, int Size)
{
	if (this->m_SocketDescriptor == 0)
	{
		throw SocketException("Socket is not created.");
		return -1;
	}

	if (this->m_Mode == Socket::UDP)
	{
		throw SocketException("Socket is set to UDP, not TCP.");
		return -1;
	}
#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
	int ReturnValue = send(this->m_SocketDescriptor, Buffer, Size, 0);
#else
	int ReturnValue = send(this->m_SocketDescriptor, (const char*)Buffer, Size, 0);
#endif
	if (ReturnValue == -1)
	{
		ProvideErrorString();
		throw SocketException(m_ErrorString);
	}

	return ReturnValue;
}

int Socket::ReadFrom(void* Buffer, int Size, bool Block)
{
	if (this->m_SocketDescriptor == 0)
	{
		throw SocketException("Socket is not created.");
		return -1;
	}

	if (this->m_Domain == Socket::TCP)
	{
		throw SocketException("Socket is set to TCP, not UDP.");
		return -1;
	}
	struct sockaddr_in DestinationAddress;
	memset(&DestinationAddress, 0, sizeof(struct sockaddr_in));
	int ReturnValue = 0;
#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
	int Linux_Recv_flag = 0;
	if (!Block) {
		Linux_Recv_flag = MSG_DONTWAIT;
	}
	socklen_t AddressStructureSize = sizeof(struct sockaddr_in);
	ReturnValue = recvfrom(this->m_SocketDescriptor, Buffer, Size, Linux_Recv_flag, (struct sockaddr*)&DestinationAddress, &AddressStructureSize);
#else
	u_long mode = 1;
	if (!Block) {
		//设置为非阻塞模式
		ioctlsocket(this->m_SocketDescriptor, FIONBIO, &mode);
	}
	int AddressStructureSize = sizeof(struct sockaddr_in);
	ReturnValue = recvfrom(this->m_SocketDescriptor, (char*)Buffer, Size, 0, (struct sockaddr*)&DestinationAddress, &AddressStructureSize);
#endif
	if (ReturnValue == -1) {
		ProvideErrorString();
		int ErrCode = 0;
		ErrCode = this->GetError();
		if (ErrCode != 0 && ErrCode != EAGAIN && ErrCode != EWOULDBLOCK)
		{
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
			if (ErrCode != WSAEWOULDBLOCK) {
				//throw SocketException(m_ErrorString);
				return 0;
			}
#else
			//throw SocketException(m_ErrorString);
			return 0;
#endif
		}
	}
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
	if (!Block) {
		mode = 0; //重新设置为阻塞模式
		ioctlsocket(this->m_SocketDescriptor, FIONBIO, &mode);
	}
#endif
	return ReturnValue;
}

int Socket::WriteTo(void* Buffer, int Size, const char* DestinationHost, unsigned short PortNumber)
{
	if (this->m_SocketDescriptor == 0)
	{
		throw SocketException("Socket is not created.");
		return -1;
	}

	if (this->m_Mode == Socket::TCP)
	{
		throw SocketException("Socket is set to TCP, not UDP.");

	}
	struct sockaddr_in DestinationAddress;

	memset(&DestinationAddress, 0, sizeof(struct sockaddr_in));

	DestinationAddress.sin_family = m_Domain;
	DestinationAddress.sin_port = htons(PortNumber);
	if (DestinationHost != NULL)

	//if (inet_aton(DestinationHost, &DestinationAddress.sin_addr) == 0)
	if(inet_pton(m_Domain, DestinationHost, &DestinationAddress.sin_addr) == 0)
	{

		throw SocketException("Invalid address.");
		return -1;
	}

#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
	int ReturnValue = sendto(this->m_SocketDescriptor, Buffer, Size, 0, (struct sockaddr*)&DestinationAddress, sizeof(struct sockaddr_in));
#else
	int ReturnValue = sendto(this->m_SocketDescriptor, (const char*)Buffer, Size, 0, (struct sockaddr*)&DestinationAddress, sizeof(struct sockaddr_in));
#endif
	if (ReturnValue == -1)
	{
		ProvideErrorString();
		throw SocketException(m_ErrorString);
	}

	return ReturnValue;
}

bool Socket::Close()
{
	if (this->m_SocketDescriptor != 0)
	{
#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
		if (close(this->m_SocketDescriptor) == -1)
#else
		if (closesocket(this->m_SocketDescriptor) == -1)
#endif
		{
			ProvideErrorString();
			throw SocketException(m_ErrorString);
			return false;
		}
		return true;
	}
	throw SocketException("Socket is not created nor connected.");
	return false;
}

SocketMode Socket::GetMode()
{
	return this->m_Mode;
}

SocketProtocol Socket::GetProtocol()
{
	return this->m_SocketProtocol;
}

int Socket::GetError() const
{
	return (this->m_ErrorFlag);
}

std::string& Socket::GetErrorString()
{
	return this->m_ErrorString;
}

SocketProtocol Socket::GetSocketProtocol(const char* Protocol)
{
	struct protoent *TempProtoStruct;
	int ProtocolNumber;
	TempProtoStruct = getprotobyname(Protocol);

	ProtocolNumber = TempProtoStruct->p_proto;
#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
	endprotoent();
#else

#endif

	return ProtocolNumber;
}
std::string Socket::GetRemoteAddr()
{
	std::string TmpRst;
	if (this->m_Domain == IPv4) {
		char *TmpName;
		in_addr MyAddr;

		#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
            MyAddr.s_addr =
		#else
            MyAddr.S_un.S_addr =
		#endif

		this->m_IPAddress;
		TmpName = inet_ntoa(MyAddr);

		TmpRst = TmpName;
		//delete[] TmpName;
		return TmpRst;

	}
	else{ // if (this->m_Domain == IPv6) {
		char *TmpName;
		in_addr MyAddr;
		#ifdef EASYCROSSPLATFORM_PLATFORM_UNIX
            MyAddr.s_addr =
		#else
            MyAddr.S_un.S_addr =
		#endif
		this->m_IPAddress;
		TmpName = inet_ntoa(MyAddr);
		TmpRst = TmpName;
		return TmpRst;
	}
}
unsigned short Socket::GetRemotePort() {
	return this->m_PortNumber;
}
