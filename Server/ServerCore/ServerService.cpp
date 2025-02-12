#include "pch.h"
#include "ServerService.h"

ServerService::ServerService(NetAddress address, shared_ptr<IocpCore> core, SessionFactory sessionFactory, int32 maxSessionCount)
	: Service(eServiceType::Server, address, core, sessionFactory, maxSessionCount)
{
}

bool ServerService::Start()
{
	if (CanStart() == false)
		return false;

	mListener = make_shared<Listener>();
	if (mListener == nullptr)
		return false;

	shared_ptr<ServerService> service = static_pointer_cast<ServerService>(shared_from_this());
	if (mListener->StartAccept(service) == false)
		return false;

	return true;
}

bool ServerService::Stop()
{
	Service::Stop();
	return true;
}
