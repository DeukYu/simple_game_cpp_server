#include "pch.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"

ServerSessionManager GServerSessionManager;

void ServerSessionManager::Add(shared_ptr<ServerSession> session)
{
	unique_lock<shared_mutex> lock(mLock);
	mSessions.insert(session);

}

void ServerSessionManager::Remove(shared_ptr<ServerSession> session)
{
	unique_lock<shared_mutex> lock(mLock);
	mSessions.erase(session);
}

void ServerSessionManager::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	unique_lock<shared_mutex> lock(mLock);
	for (auto session : mSessions)
	{
		session->Send(sendBuffer);
	}
}
