#pragma once
#include "Session.h"

class ClientSession : public Session
{
public:
	~ClientSession();

	virtual void OnConnected() override;
	virtual void OnDisconnect() override;
	virtual int32 OnRecv(byte* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
};

