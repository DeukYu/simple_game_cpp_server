#pragma once
#include "IocpCore.h"

class Session;

enum class eEventType : uint8
{
	Accept,
	Connect,
	DisConnect,
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

class DisConnectEvent : public IocpEvent
{
public:
	DisConnectEvent() : IocpEvent(eEventType::DisConnect) {}
};

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(eEventType::Recv) {}
};

class SendEvent : public IocpEvent
{
	SendEvent() : IocpEvent(eEventType::Send) {}
};