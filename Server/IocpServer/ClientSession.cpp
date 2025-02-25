#include "pch.h"
#include "ClientSession.h"
#include "ServerSessionManager.h"

ClientSession::~ClientSession()
{
	cout << "~ClientSession" << endl;
}

void ClientSession::OnConnected()
{
	GServerSessionManager.Add(static_pointer_cast<ClientSession>(shared_from_this()));
}

void ClientSession::OnDisconnect()
{
	GServerSessionManager.Remove(static_pointer_cast<ClientSession>(shared_from_this()));
}

int32 ClientSession::OnRecv(byte* buffer, int32 len)
{
	cout << "OnRecv Len = " << len << endl;

	shared_ptr<SendBuffer> sendBuffer = GSendBufferManager->Open(4096);
	::memcpy(sendBuffer->Buffer(), buffer, len);
	sendBuffer->Close(len);
	Send(sendBuffer);

	GServerSessionManager.Broadcast(sendBuffer);

	return len;
}

void ClientSession::OnSend(int32 len)
{
	cout << "OnSend Len = " << len << endl;
}
