#pragma once
#include "Session.h"

class ServerSession : public Session
{
public:
	~ServerSession();

	virtual void OnConnected() override;
	virtual void OnDisconnect() override;
	virtual int32 OnRecv(byte* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
};

