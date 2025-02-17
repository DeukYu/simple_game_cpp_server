#include "pch.h"

#include "Session.h"
#include "ClientService.h"
#include "ThreadManager.h"

char sendData[] = "Hello World";

class ServerSession : public Session
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override
	{
		cout << "OnConnected" << endl;

		shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
		sendBuffer->CopyData(sendData, sizeof(sendData));
		Send(sendBuffer);
	}

	virtual void OnDisconnect() override
	{
		cout << "OnDisconnect" << endl;
	}

	virtual int32 OnRecv(byte* buffer, int32 len) override
	{
		cout << "OnRecv Len = " << len << endl;

		this_thread::sleep_for(1s);

		shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
		sendBuffer->CopyData(sendData, sizeof(sendData));
		Send(sendBuffer);
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}
};

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