#include "pch.h"
#include "Service.h"
#include "session.h"
#include "Listener.h"

Service::Service(eServiceType type, NetAddress address, shared_ptr<IocpCore> core, SessionFactory sessionFactory, int32 maxSessionCount)
	: mServiceType(type), mAddress(address), mIocpCore(core), mSessionFactory(sessionFactory), mMaxSessionCount(maxSessionCount)
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
	shared_ptr<Session> session = mSessionFactory();

	session->SetService(shared_from_this());

	if (mIocpCore->Register(session) == false)
		return nullptr;

	return session;
}

void Service::AddSession(shared_ptr<Session> session)
{
	unique_lock<mutex> lock(mLock);
	mSessionCount++;
	mSessions.insert(session);
}

void Service::ReleaseSession(shared_ptr<Session> session)
{
	unique_lock<mutex> lock(mLock);
	
	mSessions.erase(session);
	mSessionCount--;
}