#pragma once
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

class Session;

enum class ServiceType : uint8
{
	Server,
	Client,
};

using SessionFactory = function<shared_ptr<Session>(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress address, shared_ptr<IocpCore> core ,SessionFactory sessionFactory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool Start() abstract;
	bool CanStart() { return m_sessionFactory != nullptr; }

	virtual bool Stop();
	void SetSessionFactory(SessionFactory sessionFactory) { m_sessionFactory = sessionFactory; }

	shared_ptr<Session> CreateSession();
	void AddSession(shared_ptr<Session> session);
	void ReleaseSession(shared_ptr<Session> session);
	int32 GetCurrentSessionCount() { return m_sessionCount; }
	int32 GetMaxSessionCount() { return m_maxSessionCount; }

public:
	ServiceType GetType() { return m_type; }
	NetAddress GetAddress() { return m_address; }
	shared_ptr<IocpCore>& GetIocpCore() { return m_iocpCore; }


protected:
	shared_mutex m_lock;
	ServiceType m_type;
	NetAddress m_address = {};
	shared_ptr<IocpCore> m_iocpCore;

	set<shared_ptr<Session>> m_sessions;
	int32 m_sessionCount = 0;
	int32 m_maxSessionCount = 0;
	SessionFactory m_sessionFactory;
};

class ClientService : public Service
{
public:
	ClientService(
		NetAddress address, 
		shared_ptr<IocpCore> core, 
		SessionFactory sessionFactory, 
		int32 maxSessionCount = 1);
	virtual ~ClientService() {};
	
	virtual bool Start() override;
};

class ServerService : public Service
{
public:
	ServerService(
		NetAddress address, 
		shared_ptr<IocpCore> core, 
		SessionFactory sessionFactory, 
		int32 maxSessionCount = 1);
	virtual ~ServerService() {};
	virtual bool Start() override;
	virtual bool Stop() override;

private:
	shared_ptr<Listener> m_listener;
};