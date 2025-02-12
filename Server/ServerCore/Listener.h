#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;
class ServerService;
class IocpEvent;

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public: /* �ܺο��� ��� */
	bool StartAccept(shared_ptr<ServerService> service);
	void CloseSocket();

public: /* �������̽� ���� */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private: /* ���� ���� */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET mSocket = INVALID_SOCKET;
	vector<AcceptEvent*> mAcceptEvents;
	shared_ptr<ServerService> mService = nullptr;
};

