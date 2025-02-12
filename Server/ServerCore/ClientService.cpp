#include "pch.h"
#include "ClientService.h"

ClientService::ClientService(NetAddress address, shared_ptr<IocpCore> core, SessionFactory sessionFactory, int32 maxSessionCount)
	: Service(eServiceType::Client, address, core, sessionFactory, maxSessionCount)
{
}

bool ClientService::Start()
{
	return true;
}
