
#include "pch.h"

#include "ThreadManager.h"
#include "ServerService.h"
#include "Session.h"

class GameSession : public Session
{
public:
	~GameSession()
	{
		cout << "~GameSession" << endl;
	}

	virtual int32 OnRecv(byte* buffer, int32 len) override
	{
		cout << "OnRecv Len = " << len << endl;
		Send(buffer, len);
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}
};

int main()
{
	shared_ptr<ServerService> service = make_shared<ServerService>(
		NetAddress("127.0.0.1", 7777), 
		make_shared<IocpCore>(), 
		[]() {return make_shared<GameSession>(); },
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