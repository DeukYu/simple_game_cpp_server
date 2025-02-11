#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"

Listener::~Listener()
{
	SocketUtils::Close(_socket);

	for (auto acceptEvent : _acceptEvents)
	{
		delete(acceptEvent);
	}
}

bool Listener::StartAccept(const NetAddress& address)
{
	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
	{
		return false;
	}

	if (GIocpCore.Register(this) == false)
	{
		return false;
	}

	if (SocketUtils::SetReUseAddress(_socket, true) == false)
	{
		return false;
	}

	if (SocketUtils::SetLinger(_socket, true, 0) == false)
	{
		return false;
	}

	if (SocketUtils::Bind(_socket, address) == false)
	{
		return false;
	}

	if (SocketUtils::Listen(_socket) == false)
	{
		return false;
	}

	const int32 acceptCount = 1;
	for (int32 i = 0; i < acceptCount; ++i)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->m_owner = shared_from_this();
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(_socket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	if (iocpEvent->m_eventType != IOCP_IO_TYPE::Accept)
	{
		return;
	}

	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = make_shared<Session>();

	acceptEvent->Init();
	acceptEvent->m_session = session;

	DWORD bytesReceived = 0;
	if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer, 0,
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
		OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = acceptEvent->m_session;
	
	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), & sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetAddress(NetAddress(sockAddress));

	cout << "Client Connected!" << endl;

	RegisterAccept(acceptEvent);
}
