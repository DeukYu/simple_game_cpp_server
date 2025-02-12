#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

Session::Session()
{
	mSocket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(mSocket);
}

void Session::DisConnect(string_view cause)
{
	if (mConnected.exchange(false) == false)
		return;

	cout << "DisConnect : " << cause << endl;

	// Contents Code Overloading
	OnDisconnect();

	// Close Socket
	SocketUtils::Close(mSocket);

	// Release Session
	GetService()->ReleaseSession(GetSession());
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(mSocket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// TODO : 패킷 처리
	switch (iocpEvent->mEventType)
	{
	case eEventType::Connect:
		ProcessConnect();
		break;
	case eEventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case eEventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}

}

void Session::RegisterConnect()
{
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	mRecvEvent.Init();
	mRecvEvent.mOwner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(mRecvBuffer);
	wsaBuf.len = sizeof(mRecvBuffer);

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == WSARecv(mSocket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &mRecvEvent, nullptr))
	{
		int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			HandleError(errCode);
			mRecvEvent.mOwner = nullptr;	// Release
		}
	}
}

void Session::RegisterSend()
{
}

void Session::ProcessConnect()
{
	mConnected.store(true);

	// Register Session
	GetService()->AddSession(GetSession());

	// Contents Code Overloading
	OnConnected();

	// Register Recv
	RegisterRecv();
}

void Session::ProcessRecv(int32 numOfBytes)
{
	mRecvEvent.mOwner = nullptr;	// Release

	if (numOfBytes == 0)
	{
		DisConnect("Recv 0");
		return;
	}

	// TODO
	cout << "Recv : " << numOfBytes << endl;

	// Register Recv
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
}

void Session::HandleError(int32 errCode)
{
	switch (errCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		DisConnect("HandleError");
	default:
		cout << "Handle Error : " << errCode << endl;
		break;
	}
}
