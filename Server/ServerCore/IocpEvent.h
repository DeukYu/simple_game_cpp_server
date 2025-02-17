#pragma once
#include "IocpCore.h"

class Session;

enum class eEventType : uint8
{
	Accept,
	Connect,
	Disconnect,
	Recv,
	Send,
};

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(eEventType type);

	void Init();

public:
	eEventType mEventType;
	shared_ptr<IocpObject> mOwner;
};

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(eEventType::Accept) {}

public:
	shared_ptr<Session> m_session = nullptr;
};

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(eEventType::Connect) {}
};

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(eEventType::Disconnect) {}
};

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(eEventType::Recv) {}
};

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(eEventType::Send) {}

	vector<shared_ptr<SendBuffer>> mSendBuffers;
};