
#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

class TestLock
{
public:
	int32 TestRead()
	{
		shared_lock<shared_mutex> lock(_lock);

		return num;
	}
	void TestPush()
	{
		unique_lock<shared_mutex> lock(_lock);

		++num;
		//_queue.push(rand() % 100);
	}
	void TestPop()
	{
		unique_lock<shared_mutex> lock(_lock);

		//if (_queue.empty() == false)
		//	_queue.pop();
	}
private:
	queue<int32> _queue;
	shared_mutex _lock;
	int32 num = 0;
};

TestLock testLock;

void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		//testLock.TestPop();
	}
}

void ThreadRead()
{
	while (true)
	{
		int32 value = testLock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(1ms);
	}
}

int main()
{
	for (int32 i = 0; i < 6; ++i)
	{
		GThreadManager->Launch(ThreadWrite);
	}

	for (int32 i = 0; i < 2; ++i)
	{
		GThreadManager->Launch(ThreadRead);
	}

	GThreadManager->Join();
}