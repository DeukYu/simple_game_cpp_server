#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"
#include "RecvBuffer.h"

class Service;

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;

	enum {
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	Session();
	virtual ~Session();

public: /* 외부에서 사용 */
	void Send(byte* buffer, int32 len);
	bool Connect();
	void DisConnect(string_view cause);

	shared_ptr<Service> GetService() { return mService.lock(); }
	void				SetService(shared_ptr<Service> service) { mService = service; }

public: /* 정보 관련 */
	void				SetNetAddress(const NetAddress& netAddress) { mNetAddress = netAddress; }
	NetAddress			GetNetAddress() const { return mNetAddress; }
	SOCKET				GetSocket() const { return mSocket; }
	bool				IsConnected() const { return mConnected; }
	shared_ptr<Session> GetSession() { return static_pointer_cast<Session>(shared_from_this()); }

public: /* 인터페이스 구현 */
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes) override;

private: /* 전송 관련 */
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterRecv();
	void RegisterSend(SendEvent* sendEvent);

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(SendEvent* sendEvent, int32 numOfBytes);

	void HandleError(int32 errCode);

protected:	/* 컨텐츠 코드에서 재정의 */
	virtual void OnConnected() {}
	virtual int32 OnRecv(byte* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) {}
	virtual void OnDisconnect() {}

private:
	weak_ptr<Service> mService;
	SOCKET mSocket = INVALID_SOCKET;
	NetAddress mNetAddress = {};
	atomic<bool> mConnected = false;

private:
	shared_mutex mLock;

private:	/* 수신 관련 */
	RecvBuffer mRecvBuffer;

private:
	ConnectEvent mConnectEvent;
	DisconnectEvent mDisconnectEvent;
	RecvEvent mRecvEvent;
};

