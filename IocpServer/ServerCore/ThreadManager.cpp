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
	scoped_lock<mutex> lock(_lock);

	_threads.emplace_back([this, callback]() {
		InitTLS();
		callback();
		DestroyTLS();
		});
}

void ThreadManager::Join()
{
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static atomic<uint32> s_ThreadId = 1;
	LThreadId = s_ThreadId++;
}

void ThreadManager::DestroyTLS()
{
}
