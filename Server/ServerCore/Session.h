#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();

public: /* ���� ���� */
	void		SetNetAddress(const NetAddress& netAddress) { _netAddress = netAddress; }
	NetAddress	GetNetAddress() const { return _netAddress; }
	SOCKET		GetSocket() const { return _socket; }

public: /* �������̽� ���� */
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes) override;

public:
	char _recvBuffer[1024];

private:
	SOCKET _socket = INVALID_SOCKET;
	NetAddress _netAddress = {};
	atomic<bool> _connected = false;
};

