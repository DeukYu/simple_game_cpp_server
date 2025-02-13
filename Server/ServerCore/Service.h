#pragma once
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

class Session;

enum class eServiceType : uint8
{
	Server,
	Client,
};

using SessionFactory = function<shared_ptr<Session>(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(eServiceType type, NetAddress address, shared_ptr<IocpCore> core ,SessionFactory sessionFactory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool Start() abstract;
	bool CanStart() { return mSessionFactory != nullptr; }

	virtual bool Stop();
	void SetSessionFactory(SessionFactory sessionFactory) { mSessionFactory = sessionFactory; }

	shared_ptr<Session> CreateSession();
	void AddSession(shared_ptr<Session> session);
	void ReleaseSession(shared_ptr<Session> session);
	int32 GetCurrentSessionCount() { return mSessionCount; }
	int32 GetMaxSessionCount() { return mMaxSessionCount; }

public:
	eServiceType GetServiceType() { return mServiceType; }
	NetAddress GetAddress() { return mAddress; }
	shared_ptr<IocpCore>& GetIocpCore() { return mIocpCore; }


protected:
	shared_mutex mLock;
	eServiceType mServiceType;
	NetAddress mAddress = {};
	shared_ptr<IocpCore> mIocpCore;

	set<shared_ptr<Session>> mSessions;
	int32 mSessionCount = 0;
	int32 mMaxSessionCount = 0;
	SessionFactory mSessionFactory;
};