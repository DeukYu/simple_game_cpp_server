#pragma once

#include <thread>
#include <functional>

class ThreadManager
{
public:
	ThreadManager() = default;
	~ThreadManager() = default;

	void Launch(function<void(void)> callback);

	static void InitTLS();
	static void DestroyTLS();

private:
	concurrent_vector<jthread> mThreads;
};

