#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

void ThreadManager::Launch(function<void(void)> callback)
{
	mThreads.push_back(jthread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}));
}

void ThreadManager::InitTLS()
{
	static atomic<uint32> s_ThreadId = 1;
	LThreadId = s_ThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}
