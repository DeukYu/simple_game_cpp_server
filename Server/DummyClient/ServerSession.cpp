#include "pch.h"
#include "ServerSession.h"

char sendData[] = "Hello World";

ServerSession::~ServerSession()
{
	cout << "~ServerSession" << endl;
}

void ServerSession::OnConnected()
{
	cout << "OnConnected" << endl;

	shared_ptr<SendBuffer> sendBuffer = GSendBufferManager->Open(4096);
	::memcpy(sendBuffer->Buffer(), sendData, sizeof(sendData));
	sendBuffer->Close(sizeof(sendData));
	Send(sendBuffer);
}

void ServerSession::OnDisconnect()
{
	cout << "OnDisconnect" << endl;
}

int32 ServerSession::OnRecv(byte* buffer, int32 len)
{
	cout << "OnRecv Len = " << len << endl;

	this_thread::sleep_for(1s);

	shared_ptr<SendBuffer> sendBuffer = GSendBufferManager->Open(4096);
	::memcpy(sendBuffer->Buffer(), sendData, sizeof(sendData));
	sendBuffer->Close(sizeof(sendData));
	Send(sendBuffer);
	return len;
}

void ServerSession::OnSend(int32 len)
{
	cout << "OnSend Len = " << len << endl;
}
