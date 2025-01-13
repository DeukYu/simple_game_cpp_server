#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	lock_guard<mutex> lock(_lock);

	_threads.emplace_back(thread([=]() {
		InitTLS();
		callback();
		DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())

			t.join();
	}
}

void ThreadManager::InitTLS()
{
	static atomic<uint32> s_ThreadId = 1;
	LThreadId = s_ThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}
