#include "pch.h"
#include "ServerSessionManager.h"
#include "ClientSession.h"

ServerSessionManager GServerSessionManager;

void ServerSessionManager::Add(shared_ptr<ClientSession> session)
{
	unique_lock lock(mLock);
	mSessions.insert(session);

}

void ServerSessionManager::Remove(shared_ptr<ClientSession> session)
{
	unique_lock lock(mLock);
	mSessions.erase(session);
}

void ServerSessionManager::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	shared_lock(mLock);
	for (auto session : mSessions)
	{
		session->Send(sendBuffer);
	}
}
