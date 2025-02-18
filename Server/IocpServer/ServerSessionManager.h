#pragma once

class ClientSession;

class ServerSessionManager
{
public:
	void Add(shared_ptr<ClientSession> session);
	void Remove(shared_ptr<ClientSession> session);
	void Broadcast(shared_ptr<SendBuffer> sendBuffer);

private:
	shared_mutex mLock;
	set<shared_ptr<ClientSession>> mSessions;
};

extern ServerSessionManager GServerSessionManager;

