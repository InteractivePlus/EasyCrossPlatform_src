#include "XSYDTCPSocket.h"

EasyCrossPlatform::Network::TCPSocket::TCPSocket()
	: Socket(Socket::TCP,"TCP"),
	m_Connected(false),
	m_TCPMode(TCPSocket::NotSet)
{ }

EasyCrossPlatform::Network::TCPSocket::TCPSocket(TCPSocket& TCPSocketObject)
	: Socket(TCPSocketObject),
	m_Connected(TCPSocketObject.m_Connected),
	m_PortNumber(TCPSocketObject.m_PortNumber),
	m_TCPMode(TCPSocketObject.m_TCPMode)
{ }

EasyCrossPlatform::Network::TCPSocket::TCPSocket(Socket& SocketObject)
	: Socket(SocketObject),
	m_Connected(false),
	m_PortNumber(0),
	m_TCPMode(TCPSocket::NotSet)
{ }

EasyCrossPlatform::Network::TCPSocket::TCPSocket(unsigned short PortNumber)
	: Socket(Socket::TCP,"TCP"),
	m_Connected(false),
	m_TCPMode(TCPSocket::Server)
{
	this->Create();
	this->Listen(PortNumber);
}

EasyCrossPlatform::Network::TCPSocket::TCPSocket(const char* IPAddress, unsigned short PortNumber)
	: Socket(Socket::TCP, "TCP"),
	m_Connected(true),
	m_TCPMode(TCPSocket::Client)
{
	this->Create();
	try
	{
		this->Connect(IPAddress, PortNumber);
	}
	catch (SocketException& e)
	{
		m_Connected = false;
		throw SocketException(e.what());
	}
}

EasyCrossPlatform::Network::TCPSocket::TCPSocket(std::string& IPAddress, unsigned short PortNumber)
	: Socket(Socket::TCP, "TCP"),
	m_Connected(true),
	m_TCPMode(TCPSocket::Client)
{
	this->Create();
	try
	{
		this->Connect(IPAddress, PortNumber);
	}
	catch (SocketException& e)
	{
		m_Connected = false;
		throw SocketException(e.what());
	}
}

EasyCrossPlatform::Network::TCPSocket::~TCPSocket()
{
	if (m_Connected)
		this->Disconnect();
}

int EasyCrossPlatform::Network::TCPSocket::Connect(const char* IPAddress, unsigned short PortNumber)
{
	return Socket::Connect(IPAddress, PortNumber);
}

int EasyCrossPlatform::Network::TCPSocket::Connect(std::string& IPAddress, unsigned short PortNumber)
{
	return Socket::Connect(IPAddress, PortNumber);
}

int EasyCrossPlatform::Network::TCPSocket::Disconnect()
{
	if (!m_Connected)
	{
		throw SocketException("TCPSocket is not connected!");
		return -1;
	}
	this->m_Connected = false;
	return Socket::Close();
}

EasyCrossPlatform::Network::TCPSocket* EasyCrossPlatform::Network::TCPSocket::Accept()
{
	Socket *pointer = Socket::Accept();
	if (pointer != NULL) {
		EasyCrossPlatform::Network::TCPSocket* sock = new EasyCrossPlatform::Network::TCPSocket(*pointer);
		sock->m_Connected = true;
		sock->m_TCPMode = TCPSocket::Client;
		sock->m_PortNumber = this->m_PortNumber;

		delete pointer;
		return sock;
	}
	else {
		return NULL;
	}
}

int EasyCrossPlatform::Network::TCPSocket::Send(const void* Buffer, int Size)
{
	if (m_Connected || m_TCPMode == Server)
		return Socket::Write(Buffer, Size);

	throw SocketException("TCPSocket not connected.");
	return -1;
}

int EasyCrossPlatform::Network::TCPSocket::Receive(const void* Buffer, int Size, bool BlockMode)
{
	int ReadRst = Socket::Read(Buffer, Size, BlockMode);
	if (ReadRst == 0) {
		this->m_Connected = false;
	}
	return ReadRst;
}

const bool EasyCrossPlatform::Network::TCPSocket::isConnected()
{
	return this->m_Connected;
}

bool EasyCrossPlatform::Network::TCPSocket::Close()
{
	this->m_Connected = false;
	return Socket::Close();
}


