#include "pch.h"

#include "ThreadManager.h"
#include "ClientService.h"
#include "ServerSession.h"


int main()
{
	this_thread::sleep_for(1s);

	shared_ptr<ClientService> service = make_shared<ClientService>(
		NetAddress("127.0.0.1", 7777),
		make_shared<IocpCore>(),
		[]() {return make_shared<ServerSession>(); },
		5);

	service->Start();

	for (int i = 0; i < 2; ++i)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}
	GThreadManager->Join();

}