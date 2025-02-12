﻿
#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

#include "ServerService.h"
#include "Session.h"

int main()
{
	shared_ptr<ServerService> service = make_shared<ServerService>(
		NetAddress("127.0.0.1", 7777), 
		make_shared<IocpCore>(), 
		[]() {return make_shared<Session>(); },
		100);
	
	service->Start();

	for (int i = 0; i < 5; ++i)
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