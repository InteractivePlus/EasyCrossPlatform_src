#include "XSYDAsyncTCPServer.h"

void EasyCrossPlatform::Network::TCPAsyncSocket::StartUp(bool UseSameMutex)
{
	this->m_ConnectionThread = new EasyCrossPlatform::Thread::SingleWork(&(this->SuperviseConnectionThread));
	this->m_MsgThread = new EasyCrossPlatform::Thread::SingleWork(&(this->SuperviseMsgThread));
	this->m_ConnectionThread->StartJob(NULL, this);
	this->m_MsgThread->StartJob(NULL,this);

	this->m_ConnectionMutex = new std::mutex();
	if (UseSameMutex) {
		this->m_MsgMutex = this->m_ConnectionMutex;
	}
	else {
		this->m_MsgMutex = new std::mutex();
	}
	this->m_Socket.Create();
}

void EasyCrossPlatform::Network::TCPAsyncSocket::SuperviseConnectionThread(std::thread::id ThreadID, void * ClassObj, bool * RunningSign, std::mutex * Mutex)
{
	TCPAsyncSocket &MyClass = *((TCPAsyncSocket*)ClassObj);
	TCPSocket* NewAcceptedSocket;
	while (*RunningSign) {
		if (MyClass.isListening) {
			NewAcceptedSocket = MyClass.m_Socket.Accept();
			if (NewAcceptedSocket != NULL) {
				MyClass.m_ClientSocketListMutex.lock();
				MyClass.m_ClientSockets.push_back(NewAcceptedSocket);
				MyClass.ConnectedCallBack(*(NewAcceptedSocket), *(MyClass.m_ConnectionMutex));
				MyClass.m_ClientSocketListMutex.unlock();
			}
		}
	}
}

void EasyCrossPlatform::Network::TCPAsyncSocket::SuperviseMsgThread(std::thread::id ThreadID, void * ClassObj, bool * RunningSign, std::mutex * Mutex)
{
	TCPAsyncSocket &MyClass = *((TCPAsyncSocket*)ClassObj);
	char ReadBuffer[TCPAsyncServerTmpSize + 1] = "";
	std::string ReadTmpBuffer;
	int RecieveRst = 0;
	bool FirstRecv = false;
	bool NeedDisconCB = false;
	while (*RunningSign) {
		if (MyClass.isListening) {
			if (!MyClass.m_ClientSockets.empty()) {
				for (auto i = MyClass.m_ClientSockets.begin(); i != MyClass.m_ClientSockets.end();i++) {
					//Msg Recieve Start
					FirstRecv = true;
					NeedDisconCB = false;
					RecieveRst = (*i)->Receive(ReadBuffer, TCPAsyncServerTmpSize, false);
					if (RecieveRst == -1) {
						//没有信息, 直接pass
						continue;
					}
					else if (RecieveRst == 0) {
						//接受失败, 链接已断
						MyClass.DisconnectCallBack(*(*i), *(MyClass.m_ConnectionMutex));
						delete (*i);
						MyClass.m_ClientSocketListMutex.lock();
						i = MyClass.m_ClientSockets.erase(i);
						MyClass.m_ClientSocketListMutex.unlock();
						if (i == MyClass.m_ClientSockets.end()) {
							break;
						}
						else {
							continue;
						}
					}
					else if(RecieveRst < TCPAsyncServerTmpSize) {
						ReadBuffer[RecieveRst] = '\0';
						ReadTmpBuffer = ReadBuffer;
						MyClass.MsgCallBack(*(*i), ReadTmpBuffer, *(MyClass.m_MsgMutex));
					}
					else {
						ReadBuffer[RecieveRst] = '\0';
						//接受不完全, 继续接受
						ReadTmpBuffer = ReadBuffer;
						while (RecieveRst == TCPAsyncServerTmpSize) {
							//直到接受完
							RecieveRst = (*i)->Receive(ReadBuffer, TCPAsyncServerTmpSize, false);
							if (RecieveRst > 0) {
								ReadTmpBuffer[RecieveRst] = '\0';
								ReadTmpBuffer += ReadBuffer;
							}
							else if (RecieveRst == -1) {
								break;
							}
							else { //RecieveRst = 0
								NeedDisconCB = true;
								break;
							}
						}
						MyClass.MsgCallBack(*(*i), ReadTmpBuffer, *(MyClass.m_MsgMutex));
						if (NeedDisconCB) {
							MyClass.DisconnectCallBack(*(*i), *(MyClass.m_ConnectionMutex));
							delete (*i);
							MyClass.m_ClientSocketListMutex.lock();
							i = MyClass.m_ClientSockets.erase(i);
							MyClass.m_ClientSocketListMutex.unlock();
							if (i == MyClass.m_ClientSockets.end()) {
								break;
							}
							else {
								continue;
							}
						}
					}
					//Msg Recieve Finish

					//Connection Check Start
					if (!(*i)->isConnected()) {
						MyClass.DisconnectCallBack(*(*i), *(MyClass.m_ConnectionMutex));
						delete (*i);
						MyClass.m_ClientSocketListMutex.lock();
						i = MyClass.m_ClientSockets.erase(i);
						MyClass.m_ClientSocketListMutex.unlock();
						if (i == MyClass.m_ClientSockets.end()) {
							break;
						}
						else {
							continue;
						}
					}
					//Connection Check Finish
				}
			}
		}
	}
}

EasyCrossPlatform::Network::TCPAsyncSocket::TCPAsyncSocket(bool UseSameMutex)
{
	this->isListening = false;
	this->StartUp(UseSameMutex);
}

EasyCrossPlatform::Network::TCPAsyncSocket::TCPAsyncSocket(unsigned short PortNumber, bool UseSameMutex)
{
	this->m_PortNum = PortNumber;
	this->isListening = false;
	this->StartUp(UseSameMutex);
}

EasyCrossPlatform::Network::TCPAsyncSocket::TCPAsyncSocket(TCPAsyncSocket & RightObject)
{
	this->m_Socket = RightObject.m_Socket;
	this->m_ClientSockets = RightObject.m_ClientSockets;
	this->isListening = RightObject.isListening;
	this->m_PortNum = RightObject.m_PortNum;
	this->StartUp((RightObject.m_ConnectionMutex == RightObject.m_MsgMutex));
}

EasyCrossPlatform::Network::TCPAsyncSocket::~TCPAsyncSocket()
{
	if (this->m_ConnectionThread != NULL) {
		this->m_ConnectionThread->StopJob();
		delete this->m_ConnectionThread;
		this->m_ConnectionThread = NULL;
	}
	if (this->m_MsgThread != NULL) {
		this->m_MsgThread->StopJob();
		delete this->m_MsgThread;
		this->m_MsgThread;
	}


	if (this->m_ConnectionMutex != this->m_MsgMutex) {
		delete this->m_MsgMutex;
	}
	delete this->m_ConnectionMutex;
	this->m_ConnectionMutex = NULL;
	this->m_MsgMutex = NULL;

	//Cleanup ClientSockets
	this->m_ClientSocketListMutex.lock();
	if (!this->m_ClientSockets.empty()) {
		for (auto i = this->m_ClientSockets.begin(); i != this->m_ClientSockets.end(); i++) {
			delete (*i);
		}
	}
	this->m_ClientSockets.clear();
	this->m_ClientSocketListMutex.unlock();
}

bool EasyCrossPlatform::Network::TCPAsyncSocket::Listen()
{
	if (this->m_PortNum == 0) {
		return false;
	}
	if (!this->m_hasBind) {
		this->m_Socket.Bind(this->m_PortNum);
		this->m_hasBind = true;
	}
	this->m_Socket.Listen();
	this->isListening = true;
}

bool EasyCrossPlatform::Network::TCPAsyncSocket::Listen(unsigned short PortNumber)
{
	this->m_PortNum = PortNumber;
	return this->Listen();
}

bool EasyCrossPlatform::Network::TCPAsyncSocket::StopListen()
{
	if (this->isListening) {
		this->m_Socket.Close();
		this->m_ClientSocketListMutex.lock();
		if (!this->m_ClientSockets.empty()) {
			for (auto i = this->m_ClientSockets.begin(); i != this->m_ClientSockets.end(); i++) {
				delete (*i);
			}
		}
		this->m_ClientSockets.clear();
		this->m_ClientSocketListMutex.unlock();
		return true;
	}
	else {
		return false;
	}
}

void EasyCrossPlatform::Network::TCPAsyncSocket::ConnectedCallBack(TCPSocket & MySocket, std::mutex & ConnectionMutex)
{
	return;
}

void EasyCrossPlatform::Network::TCPAsyncSocket::MsgCallBack(TCPSocket & MySocket, std::string Msg, std::mutex & MsgMutex)
{
	return;
}

void EasyCrossPlatform::Network::TCPAsyncSocket::DisconnectCallBack(TCPSocket & MySocket, std::mutex & ConnectionMutex)
{
	return;
}
