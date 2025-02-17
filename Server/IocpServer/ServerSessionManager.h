#pragma once

class ServerSession;

class ServerSessionManager
{
public:
	void Add(shared_ptr<ServerSession> session);
	void Remove(shared_ptr<ServerSession> session);
	void Broadcast(shared_ptr<SendBuffer> sendBuffer);

private:
	shared_mutex mLock;
	set<shared_ptr<ServerSession>> mSessions;
};

extern ServerSessionManager GServerSessionManager;

