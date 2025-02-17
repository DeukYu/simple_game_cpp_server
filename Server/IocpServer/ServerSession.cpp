#include "pch.h"
#include "ServerSession.h"
#include "ServerSessionManager.h"

ServerSession::~ServerSession()
{
	cout << "~ServerSession" << endl;
}

void ServerSession::OnConnected()
{
	GServerSessionManager.Add(static_pointer_cast<ServerSession>(shared_from_this()));
}

void ServerSession::OnDisconnect()
{
	GServerSessionManager.Remove(static_pointer_cast<ServerSession>(shared_from_this()));
}

int32 ServerSession::OnRecv(byte* buffer, int32 len)
{
	cout << "OnRecv Len = " << len << endl;

	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	sendBuffer->CopyData(buffer, len);
	Send(sendBuffer);

	GServerSessionManager.Broadcast(sendBuffer);

	return len;
}

void ServerSession::OnSend(int32 len)
{
	cout << "OnSend Len = " << len << endl;
}
