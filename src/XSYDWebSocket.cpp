#include <XSYDWebSocket.h>
#include <iostream>

// WebSocket, v1.00 2012-09-13
//
// Description: WebSocket RFC6544 codec, written in C++.
// Homepage: http://katzarsky.github.com/WebSocket
// Author: katzarsky@gmail.com

//#include <md5/md5.h>
#include <websocketpp/base64/base64.h>
#include <websocketpp/sha1/sha1.h>

#include <string>
#include <vector>

using namespace std;
using namespace EasyCrossPlatform::Network::Websocket;

WebSocket::WebSocket() {

}

WebSocketFrameType WebSocket::parseHandshake(const EasyCrossPlatform::Parser::HTTP::HTTPRequest& Request)
{

	this->resource = Request.RequestedURL;
	
	auto OriginIt = Request.FieldsValues.find("Origin");
	if (OriginIt != Request.FieldsValues.cend()) {
		this->origin = (*OriginIt).second[0];
	}
	auto KeyIt = Request.FieldsValues.find("Sec-Websocket-Key");
	if (KeyIt != Request.FieldsValues.cend()) {
		this->key = (*KeyIt).second[0];
	}
	auto ProtocolIt = Request.FieldsValues.find("Sec-Websocket-Protocol");
	if (ProtocolIt != Request.FieldsValues.cend()) {
		this->protocol = (*ProtocolIt).second[0];
	}
	//this->key = "dGhlIHNhbXBsZSBub25jZQ==";
	//printf("PARSED_KEY:%s \n", this->key.data());

	//return FrameType::OPENING_FRAME;
	//printf("HANDSHAKE-PARSED\n");
	return OPENING_FRAME;
}

string WebSocket::trim(string str)
{
	//printf("TRIM\n");
	char* whitespace = " \t\r\n";
	string::size_type pos = str.find_last_not_of(whitespace);
	if (pos != string::npos) {
		str.erase(pos + 1);
		pos = str.find_first_not_of(whitespace);
		if (pos != string::npos) str.erase(0, pos);
	}
	else {
		return string();
	}
	return str;
}

vector<string> WebSocket::explode(
	string  theString,
	string  theDelimiter,
	bool    theIncludeEmptyStrings)
{
	//printf("EXPLODE\n");
	//UASSERT( theDelimiter.size(), >, 0 );

	vector<string> theStringVector;
	int  start = 0, end = 0, length = 0;

	while (end != string::npos)
	{
		end = theString.find(theDelimiter, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		length = (end == string::npos) ? string::npos : end - start;

		if (theIncludeEmptyStrings
			|| ((length > 0) /* At end, end == length == string::npos */
				&& (start  < theString.size())))
			theStringVector.push_back(theString.substr(start, length));

		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = ((end >(string::npos - theDelimiter.size()))
			? string::npos : end + theDelimiter.size());
	}
	return theStringVector;
}

string WebSocket::answerHandshake()
{
	unsigned char digest[20]; // 160 bit sha1 digest

	string answer;
	answer += "HTTP/1.1 101 Switching Protocols\r\n";
	answer += "Upgrade: WebSocket\r\n";
	answer += "Connection: Upgrade\r\n";
	if (this->key.length() > 0) {
		string accept_key;
		accept_key += this->key;
		accept_key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"; //RFC6544_MAGIC_KEY

															  //printf("INTERMEDIATE_KEY:(%s)\n", accept_key.data());

		SHA1 sha;
		sha.Input(accept_key.data(), accept_key.size());
		sha.Result((unsigned*)digest);

		//printf("DIGEST:"); for(int i=0; i<20; i++) printf("%02x ",digest[i]); printf("\n");

		//little endian to big endian
		for (int i = 0; i<20; i += 4) {
			unsigned char c;

			c = digest[i];
			digest[i] = digest[i + 3];
			digest[i + 3] = c;

			c = digest[i + 1];
			digest[i + 1] = digest[i + 2];
			digest[i + 2] = c;
		}

		//printf("DIGEST:"); for(int i=0; i<20; i++) printf("%02x ",digest[i]); printf("\n");

		accept_key = base64_encode((const unsigned char *)digest, 20); //160bit = 20 bytes/chars

		answer += "Sec-WebSocket-Accept: " + (accept_key)+"\r\n";
	}
	if (this->protocol.length() > 0) {
		answer += "Sec-WebSocket-Protocol: " + (this->protocol) + "\r\n";
	}
	answer += "\r\n";
	return answer;

	//return WS_OPENING_FRAME;
}

int WebSocket::makeFrame(WebSocketFrameType frame_type, unsigned char* msg, int msg_length, unsigned char* buffer, int buffer_size)
{
	int pos = 0;
	int size = msg_length;
	buffer[pos++] = (unsigned char)frame_type; // text frame

	if (size <= 125) {
		buffer[pos++] = size;
	}
	else if (size <= 65535) {
		buffer[pos++] = 126; //16 bit length follows

		buffer[pos++] = (size >> 8) & 0xFF; // leftmost first
		buffer[pos++] = size & 0xFF;
	}
	else { // >2^16-1 (65535)
		buffer[pos++] = 127; //64 bit length follows

							 // write 8 bytes length (significant first)

							 // since msg_length is int it can be no longer than 4 bytes = 2^32-1
							 // padd zeroes for the first 4 bytes
		for (int i = 3; i >= 0; i--) {
			buffer[pos++] = 0;
		}
		// write the actual 32bit msg_length in the next 4 bytes
		for (int i = 3; i >= 0; i--) {
			buffer[pos++] = ((size >> 8 * i) & 0xFF);
		}
	}
	memcpy((void*)(buffer + pos), msg, size);
	return (size + pos);
}

WebSocketFrameType WebSocket::getFrame(unsigned char* in_buffer, int& in_length, unsigned char* out_buffer, int out_size, int* out_length)
{
	//printf("getTextFrame()\n");
	if (in_length < 3) return INCOMPLETE_FRAME;

	unsigned char msg_opcode = in_buffer[0] & 0x0F;
	unsigned char msg_fin = (in_buffer[0] >> 7) & 0x01;
	unsigned char msg_masked = (in_buffer[1] >> 7) & 0x01;

	// *** message decoding 

	int payload_length = 0;
	int pos = 2;
	int length_field = in_buffer[1] & (~0x80);
	unsigned int mask = 0;

	//printf("IN:"); for(int i=0; i<20; i++) printf("%02x ",buffer[i]); printf("\n");

	if (length_field <= 125) {
		payload_length = length_field;
	}
	else if (length_field == 126) { //msglen is 16bit!
									//payload_length = in_buffer[2] + (in_buffer[3]<<8);
		payload_length = (
			(in_buffer[2] << 8) |
			(in_buffer[3])
			);
		pos += 2;
	}
	else if (length_field == 127) { //msglen is 64bit!
		payload_length = (
			(in_buffer[2] << 56) |
			(in_buffer[3] << 48) |
			(in_buffer[4] << 40) |
			(in_buffer[5] << 32) |
			(in_buffer[6] << 24) |
			(in_buffer[7] << 16) |
			(in_buffer[8] << 8) |
			(in_buffer[9])
			);
		pos += 8;
	}

	//printf("PAYLOAD_LEN: %08x\n", payload_length);
	if (in_length < payload_length + pos) {
		return INCOMPLETE_FRAME;
	}

	if (msg_masked) {
		mask = *((unsigned int*)(in_buffer + pos));
		//printf("MASK: %08x\n", mask);
		pos += 4;

		// unmask data:
		unsigned char* c = in_buffer + pos;
		for (int i = 0; i<payload_length; i++) {
			c[i] = c[i] ^ ((unsigned char*)(&mask))[i % 4];
		}
	}

	if (payload_length > out_size) {
		//TODO: if output buffer is too small -- ERROR or resize(free and allocate bigger one) the buffer ?
	}

	memcpy((void*)out_buffer, (void*)(in_buffer + pos), payload_length);
	out_buffer[payload_length] = 0;
	*out_length = payload_length;
	in_length = payload_length + pos;
	//printf("TEXT: %s\n", out_buffer);

	if (msg_opcode == 0x0) return (msg_fin) ? TEXT_FRAME : INCOMPLETE_TEXT_FRAME; // continuation frame ?
	if (msg_opcode == 0x1) return (msg_fin) ? TEXT_FRAME : INCOMPLETE_TEXT_FRAME;
	if (msg_opcode == 0x2) return (msg_fin) ? BINARY_FRAME : INCOMPLETE_BINARY_FRAME;
	if (msg_opcode == 0x9) return PING_FRAME;
	if (msg_opcode == 0xA) return PONG_FRAME;

	return ERROR_FRAME;
}


void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::Connect()
{
	throw std::runtime_error("You cannot connect using this class");
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::Disconnect()
{
	if (this->m_Shaked) {
		//Make a closing frame
		/*
		unsigned char mCloseHolder[500];
		int NewPongLength = this->mParser.makeFrame(WebSocketFrameType::CLOSING_FRAME, reinterpret_cast<unsigned char*>(""), 0, mCloseHolder, 500);
		std::string SendbackMsg = std::string(reinterpret_cast<char*>(mCloseHolder), NewPongLength);
		this->m_SocketHandle->SendMsg(SendbackMsg);
		*/
	}
	this->m_SocketHandle->Disconnect();
	this->m_Shaked = false;
}

bool EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::isConnected()
{
	return this->m_Shaked;
}

bool EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::lastMsgIsBinary()
{
	return this->m_lastMsgIsBinary;
}

EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::~WebsocketSingleConnection()
{
	this->Destroy();
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::Init()
{
	this->m_SocketHandle->SetConnectCallBack(WebsocketSingleConnection::m_Socket_Connection);
	this->m_SocketHandle->SetDisconnectCallBack(WebsocketSingleConnection::m_Socket_Disconn);
	this->m_SocketHandle->SetErrorCallBack(WebsocketSingleConnection::m_Socket_Error);
	this->m_SocketHandle->SetMsgCallBack(WebsocketSingleConnection::m_Socket_Msg);
	this->m_SocketHandle->setCustomData(this);
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::Destroy()
{
	if (this->m_deleteSocketAfter) {
		if (this->m_SocketHandle != NULL) {
			delete this->m_SocketHandle;
			this->m_SocketHandle = NULL;
		}
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onConnect(bool Succeed)
{
	if (this->m_ConnectCallBack != NULL) {
		this->m_ConnectCallBack(Succeed, this);
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onMsg(const std::string & Msg)
{
	if (this->m_MsgCallBack != NULL) {
		this->m_MsgCallBack(EasyCrossPlatform::Parser::StringUtil::toBytes(Msg), this);
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onDisconn()
{
	this->m_Shaked = false;
	if (this->m_DisconnCallBack != NULL) {
		this->m_DisconnCallBack(this);
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onError(int errNo, const std::string & errDescription)
{
	if (this->m_ErrCallBack != NULL) {
		this->m_ErrCallBack(errNo, errDescription, this);
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::onPong()
{
	if (this->PongFrameTime != 0.0) {
		double CurrentTime = EasyCrossPlatform::Runtime::Chrono::accurateTimer();
		double TimeDiff = CurrentTime - this->PongFrameTime;
		if (this->m_LatencyCallBack != NULL) {
			this->m_LatencyCallBack(TimeDiff, this);
		}
		this->PongFrameTime = 0.0;
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::m_Socket_Connection(bool Succeed, Socket::StandardClientSocket * SocketPtr)
{
	//This function never get called.
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::m_Socket_Msg(const std::vector<byte>& Msg, Socket::StandardClientSocket * SocketPtr)
{
	WebsocketSingleConnection* myWSClient = (WebsocketSingleConnection*)SocketPtr->getCustomData();
	for (byte i : Msg) {
		myWSClient->BufferWaitingForRead.push_back(i);
	}
	if (myWSClient->m_Shaked) {
		while (myWSClient->checkNewMsg());
	}
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::m_Socket_Disconn(Socket::StandardClientSocket * SocketPtr)
{
	WebsocketSingleConnection* myWSClient = (WebsocketSingleConnection*)SocketPtr->getCustomData();
	myWSClient->onDisconn();
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::m_Socket_Error(int errNo, const std::string & errDescription, Socket::StandardClientSocket * SocketPtr)
{
	WebsocketSingleConnection* myWSClient = (WebsocketSingleConnection*)SocketPtr->getCustomData();
	myWSClient->onError(errNo, errDescription);
}

bool EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::checkNewMsg()
{
	unsigned int BufferLength = this->BufferWaitingForRead.size();
	unsigned char* WaitingReadBuffer = new unsigned char[BufferLength];
	unsigned char* DecodedMsgBuffer = new unsigned char[BufferLength];
	for (byte i = 0U; i < this->BufferWaitingForRead.size(); i++) {
		WaitingReadBuffer[i] = this->BufferWaitingForRead[i];
	}
	bool bResult = false;
	int NewLength = static_cast<int>(BufferLength);
	int OutLength = 0;
	WebSocketFrameType fType = this->mParser.getFrame(WaitingReadBuffer, NewLength, DecodedMsgBuffer, NewLength - 16, &OutLength);
	
	std::string StrResult;
	int NewPongLength = 0;
	switch (fType) {
	case WebSocketFrameType::BINARY_FRAME:
	case WebSocketFrameType::TEXT_FRAME:
		StrResult = std::string(reinterpret_cast<char*>(DecodedMsgBuffer), static_cast<unsigned int>(OutLength));
		this->m_lastMsgIsBinary = fType == WebSocketFrameType::BINARY_FRAME;
		this->onMsg(StrResult);
		bResult = true;
		break;
	case WebSocketFrameType::INCOMPLETE_BINARY_FRAME:
	case WebSocketFrameType::INCOMPLETE_FRAME:
	case WebSocketFrameType::INCOMPLETE_TEXT_FRAME:
		bResult = false;
		break;
	case WebSocketFrameType::PING_FRAME:
		//Make a PONG Frame
		unsigned char mPongHolder[500];
		NewPongLength = this->mParser.makeFrame(WebSocketFrameType::PONG_FRAME, reinterpret_cast<unsigned char*>(""), 0, mPongHolder, 500);
		this->m_SocketHandle->SendMsg(std::string(reinterpret_cast<char*>(mPongHolder), NewPongLength));
		bResult = true;
		break;
	case WebSocketFrameType::PONG_FRAME:
		//Client responds!
		this->onPong();
		bResult = true;
		break;
	case WebSocketFrameType::CLOSING_FRAME:
		//Close the connection!
		this->m_Shaked = false;
		this->Disconnect();
		break;
	case WebSocketFrameType::OPENING_FRAME:
		bResult = true;
		break;
	default:
		bResult = false;
		//this->onError(-1, "Websocket Parse Error");
		this->Disconnect();
		break;
	}
	if (bResult) {
		for (int i = 0; i < NewLength; i++) {
			//Delete all used inputs
			this->BufferWaitingForRead.pop_front();
		}
	}
	if (this->BufferWaitingForRead.empty()) {
		bResult = false;
	}
	return bResult;
}

EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::WebsocketSingleConnection(const EasyCrossPlatform::Parser::HTTP::HTTPRequest& HandshakeRequest, Socket::StandardClientSocket * ClientSocket, bool DeleteSocketAfter)
{
	this->m_SocketHandle = ClientSocket;
	this->m_deleteSocketAfter = DeleteSocketAfter;

	//HandShake!
	mParser.parseHandshake(HandshakeRequest);
	std::string SendbackMsg = mParser.answerHandshake();
	this->WebsocketURL = mParser.resource;
	this->m_SocketHandle->SendMsg(SendbackMsg);
	this->m_Shaked = true;
	this->Init();
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetConnectCallBack(Socket::StandardClientConnCallBack mCB)
{
	this->m_ConnectCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetMsgCallBack(Socket::StandardClientMsgCallBack mCB)
{
	this->m_MsgCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetDisconnectCallBack(Socket::StandardClientDisconnectCallBack mCB)
{
	this->m_DisconnCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetErrorCallBack(Socket::StandardClientErrorCallBack mCB)
{
	this->m_ErrCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SetLatencyTestCallBack(WebsocketLatencyTestCallBack mCB)
{
	this->m_LatencyCallBack = mCB;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::TestLatency()
{
	if (!this->m_Shaked) {
		throw std::runtime_error("You cannot test latency while the connection is closed");
	}
	//Send a Ping Frame!
	unsigned char mPingHolder[500];
	int NewPongLength = this->mParser.makeFrame(WebSocketFrameType::PING_FRAME, reinterpret_cast<unsigned char*>(""), 0, mPingHolder, 500);
	this->m_SocketHandle->SendMsg(std::string(reinterpret_cast<char*>(mPingHolder), NewPongLength));
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SendMsg(const std::string & Msg)
{
	if (!this->m_Shaked) {
		throw std::runtime_error("You cannot send an msg while the connection is closed");
	}
	unsigned int MsgLength = Msg.length();
	unsigned char* mMsgArr = new unsigned char[MsgLength];
	unsigned char* mEncodedArr = new unsigned char[MsgLength];
	memcpy(mMsgArr, Msg.data(), MsgLength);
	int NewMsgLength = this->mParser.makeFrame(WebSocketFrameType::TEXT_FRAME, mMsgArr, static_cast<int>(MsgLength), mEncodedArr, MsgLength);
	std::string StrToSend = std::string(reinterpret_cast<char*>(mEncodedArr), static_cast<unsigned int>(NewMsgLength));
	try {
		this->m_SocketHandle->SendMsg(StrToSend);
	}
	catch (runtime_error e) {
		throw e;
	}
	delete[] mMsgArr;
	delete[] mEncodedArr;
	return;
}

void EasyCrossPlatform::Network::Websocket::WebsocketSingleConnection::SendMsg(const std::vector<byte>& Msg)
{
	if (!this->m_Shaked) {
		throw std::runtime_error("You cannot send an msg while the connection is closed");
	}
	unsigned int MsgLength = Msg.size();
	unsigned char* mMsgArr = new unsigned char[MsgLength];
	unsigned char* mEncodedArr = new unsigned char[MsgLength];
	memcpy(mMsgArr, Msg.data(), MsgLength);
	int NewMsgLength = this->mParser.makeFrame(WebSocketFrameType::BINARY_FRAME, mMsgArr, static_cast<int>(MsgLength), mEncodedArr, MsgLength);
	std::string StrToSend = std::string(reinterpret_cast<char*>(mEncodedArr), static_cast<unsigned int>(NewMsgLength));
	try {
		this->m_SocketHandle->SendMsg(StrToSend);
	}
	catch (runtime_error e) {
		throw e;
	}
	delete[] mMsgArr;
	delete[] mEncodedArr;
	return;
}
