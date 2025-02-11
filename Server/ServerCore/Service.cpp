#include "pch.h"
#include "Service.h"
#include "session.h"
#include "Listener.h"

Service::Service(ServiceType type, NetAddress address, shared_ptr<IocpCore> core, SessionFactory sessionFactory, int32 maxSessionCount)
	: m_type(type), m_address(address), m_iocpCore(core), m_sessionFactory(sessionFactory), m_maxSessionCount(maxSessionCount)
{
}

Service::~Service()
{
}

bool Service::Stop()
{
	return true;
}

shared_ptr<Session> Service::CreateSession()
{
	shared_ptr<Session> session = m_sessionFactory();

	if (m_iocpCore->Register(session) == false)
		return nullptr;

	return session;
}

void Service::AddSession(shared_ptr<Session> session)
{
	unique_lock<shared_mutex> lock(m_lock);
	m_sessionCount++;
	m_sessions.insert(session);
}

void Service::ReleaseSession(shared_ptr<Session> session)
{
	unique_lock<shared_mutex> lock(m_lock);
	m_sessionCount--;
	m_sessions.erase(session);
}

ClientService::ClientService(NetAddress address, shared_ptr<IocpCore> core, SessionFactory sessionFactory, int32 maxSessionCount)
	: Service(ServiceType::Client, address, core, sessionFactory, maxSessionCount)
{
}

bool ClientService::Start()
{
	return true;
}

ServerService::ServerService(NetAddress address, shared_ptr<IocpCore> core, SessionFactory sessionFactory, int32 maxSessionCount)
	: Service(ServiceType::Server, address, core, sessionFactory, maxSessionCount)
{
}

bool ServerService::Start()
{
	if (CanStart() == false)
		return false;

	m_listener = make_shared<Listener>();
	if (m_listener == nullptr)
		return false;

	shared_ptr<ServerService> service = static_pointer_cast<ServerService>(shared_from_this());
	if (m_listener->StartAccept(service) == false)
		return false;

	return true;
}

bool ServerService::Stop()
{
	Service::Stop();
	return true;
}
