#include "pch.h"
#include "ClientService.h"
#include "Session.h"

ClientService::ClientService(NetAddress address, shared_ptr<IocpCore> core, SessionFactory sessionFactory, int32 maxSessionCount)
	: Service(eServiceType::Client, address, core, sessionFactory, maxSessionCount)
{
}

bool ClientService::Start()
{
	if (CanStart() == false)
		return false;

	const int32 sessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < sessionCount; ++i)
	{
		shared_ptr<Session> session = CreateSession();
		if (session == nullptr)
			return false;

		if (session->Connect() == false)
			return false;

	}

	return true;
}
