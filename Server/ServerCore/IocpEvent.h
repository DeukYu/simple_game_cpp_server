#pragma once

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
	IOCP_IO_TYPE GetType() const { return _type; }

protected:
	IOCP_IO_TYPE _type;
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

	void SetSession(Session* session) { _session = session; }
	Session* GetSession() const { return _session; }
private:
	Session* _session = nullptr;
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