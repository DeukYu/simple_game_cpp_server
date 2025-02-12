#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "ServerService.h"

Listener::~Listener()
{
	CloseSocket();

	for (auto acceptEvent : mAcceptEvents)
	{
		delete(acceptEvent);
	}
}

bool Listener::StartAccept(shared_ptr<ServerService> service)
{
	mService = service;
	if (mService == nullptr)
	{
		return false;
	}

	mSocket = SocketUtils::CreateSocket();
	if (mSocket == INVALID_SOCKET)
	{
		return false;
	}

	if (mService->GetIocpCore()->Register(shared_from_this()) == false)
	{
		return false;
	}

	if (SocketUtils::SetReUseAddress(mSocket, true) == false)
	{
		return false;
	}

	if (SocketUtils::SetLinger(mSocket, true, 0) == false)
	{
		return false;
	}

	if (SocketUtils::Bind(mSocket, mService->GetAddress()) == false)
	{
		return false;
	}

	if (SocketUtils::Listen(mSocket) == false)
	{
		return false;
	}

	const int32 acceptCount = mService->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; ++i)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->mOwner = shared_from_this();
		mAcceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(mSocket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(mSocket);
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	if (iocpEvent->mEventType != eEventType::Accept)
	{
		return;
	}

	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = mService->CreateSession();	// Register IOCP

	acceptEvent->Init();
	acceptEvent->m_session = session;

	DWORD bytesReceived = 0;
	if (false == SocketUtils::AcceptEx(mSocket, session->GetSocket(), session->_recvBuffer, 0,
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
	
	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), mSocket))
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
