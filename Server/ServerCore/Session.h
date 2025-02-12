#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

class Service;

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;

public:
	Session();
	virtual ~Session();

public:
	void DisConnect(string_view cause);

	shared_ptr<Service> GetService() { return mService.lock(); }
	void				SetService(shared_ptr<Service> service) { mService = service; }

public: /* ���� ���� */
	void				SetNetAddress(const NetAddress& netAddress) { mNetAddress = netAddress; }
	NetAddress			GetNetAddress() const { return mNetAddress; }
	SOCKET				GetSocket() const { return mSocket; }
	bool				IsConnected() const { return mConnected; }
	shared_ptr<Session> GetSession() { return static_pointer_cast<Session>(shared_from_this()); }

public: /* �������̽� ���� */
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes) override;

private: /* ���� ���� */
	void RegisterConnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

	void HandleError(int32 errCode);

protected:	/* ������ �ڵ忡�� �����ε� */
	virtual void OnConnected() {}
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) {}
	virtual void OnDisconnect() {}

public:
	char mRecvBuffer[1024];

private:
	weak_ptr<Service> mService;
	SOCKET mSocket = INVALID_SOCKET;
	NetAddress mNetAddress = {};
	atomic<bool> mConnected = false;

private:
	shared_mutex mLock;

private:
	RecvEvent mRecvEvent;
};

