#pragma once
#include "IocpCore.h"

class Session;

enum class IOCP_IO_TYPE : uint8
{
	Connect,
	Accept,
	Recv,
	Send,
};

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(IOCP_IO_TYPE type);

	void Init();

public:
	IOCP_IO_TYPE m_eventType;
	shared_ptr<IocpObject> m_owner;
};

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(IOCP_IO_TYPE::Connect) {}
};

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(IOCP_IO_TYPE::Accept) {}

public:
	shared_ptr<Session> m_session = nullptr;
};

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(IOCP_IO_TYPE::Recv) {}
};

class SendEvent : public IocpEvent
{
	SendEvent() : IocpEvent(IOCP_IO_TYPE::Send) {}
};