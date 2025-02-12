#pragma once

#include "Service.h"

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
	shared_ptr<Listener> mListener;
};