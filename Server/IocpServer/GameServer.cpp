
#include "pch.h"
#include <iostream>
#include "ThreadManager.h"


#include "SocketUtils.h"
#include "Listener.h"

int main()
{
	Listener listener;
	listener.StartAccept(NetAddress("127.0.0.1", 7777));

	for (int i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					GIocpCore.Dispatch();
				}
			});
	}
	GThreadManager->Join();
}