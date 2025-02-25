#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

#include "SocketUtils.h"
#include "SendBuffer.h"
#include "SendBufferManager.h"

ThreadManager* GThreadManager = nullptr;
SendBufferManager* GSendBufferManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GSendBufferManager = new SendBufferManager();

		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GSendBufferManager;

		SocketUtils::Clear();
	}
} GCoreGlobal;