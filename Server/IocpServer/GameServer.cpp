
#include "pch.h"
#include <iostream>
#include "ThreadManager.h"


#include "SocketUtils.h"
int main()
{
	SOCKET socket = SocketUtils::CreateSocket();

	SocketUtils::BindAnyAddress(socket, 9000);

	SocketUtils::Listen(socket);

	SOCKET clientSocket = accept(socket, nullptr, nullptr);

	cout << "Client Connected" << endl;

	while (true)
	{

	}

	GThreadManager->Join();
}