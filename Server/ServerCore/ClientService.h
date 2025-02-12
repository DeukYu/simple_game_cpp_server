#pragma once
#include "Service.h"

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

