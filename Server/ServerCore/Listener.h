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

public: /* 외부에서 사용 */
	bool StartAccept(shared_ptr<ServerService> service);
	void CloseSocket();

public: /* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private: /* 수신 관련 */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET mSocket = INVALID_SOCKET;
	vector<AcceptEvent*> mAcceptEvents;
	shared_ptr<ServerService> mService = nullptr;
};

