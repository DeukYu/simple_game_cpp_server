#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

Session::Session()
	: mRecvBuffer(BUFFER_SIZE)
{
	mSocket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(mSocket);
}

void Session::Send(byte* buffer, int32 len)
{
	SendEvent* sendEvent = new SendEvent();
	sendEvent->mOwner = shared_from_this();
	sendEvent->mBuffer.resize(len);
	memcpy(sendEvent->mBuffer.data(), buffer, len);
	
	unique_lock<shared_mutex> lock(mLock);
	RegisterSend(sendEvent);
}

bool Session::Connect()
{
	return RegisterConnect();
}

void Session::DisConnect(string_view cause)
{
	if (mConnected.exchange(false) == false)
		return;

	cout << "DisConnect : " << cause << endl;

	// Contents Code OverRiding
	OnDisconnect();

	// Release Session
	GetService()->ReleaseSession(GetSession());

	RegisterDisconnect();
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
	case eEventType::Disconnect:
		ProcessDisconnect();
		break;
	case eEventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case eEventType::Send:
		ProcessSend(static_cast<SendEvent*>(iocpEvent), numOfBytes);
		break;
	default:
		break;
	}

}

bool Session::RegisterConnect()
{
	if (IsConnected())
		return false;

	if (GetService()->GetServiceType() != eServiceType::Client)
		return false;

	if (SocketUtils::SetReUseAddress(mSocket, true) == false)
		return false;

	if (SocketUtils::BindAnyAddress(mSocket, 0) == false)
		return false;

	mConnectEvent.Init();
	mConnectEvent.mOwner = shared_from_this();

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetAddress().GetSockAddr();
	if (false == SocketUtils::ConnectEx(mSocket, reinterpret_cast<SOCKADDR*>(&sockAddr),
		sizeof(sockAddr), nullptr, 0, &numOfBytes, &mConnectEvent))
	{
		int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			HandleError(errCode);
			mConnectEvent.mOwner = nullptr;	// Release
			return false;
		}
	}
	return true;
}

bool Session::RegisterDisconnect()
{
	mDisconnectEvent.Init();
	mDisconnectEvent.mOwner = shared_from_this();

	if (false == SocketUtils::DisconnectEx(mSocket, &mDisconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			HandleError(errCode);
			mDisconnectEvent.mOwner = nullptr;	// Release
			return false;
		}
	}

	return true;
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	mRecvEvent.Init();
	mRecvEvent.mOwner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(mRecvBuffer.WritePos());
	wsaBuf.len = mRecvBuffer.FreeSize();

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

void Session::RegisterSend(SendEvent* sendEvent)
{
	if (IsConnected() == false)
	{
		return;
	}

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(sendEvent->mBuffer.data());
	wsaBuf.len = static_cast<ULONG>(sendEvent->mBuffer.size());

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == WSASend(mSocket, &wsaBuf, 1, OUT & numOfBytes, 0, sendEvent, nullptr))
	{
		int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			HandleError(errCode);
			sendEvent->mOwner = nullptr;	// Release
			delete(sendEvent);
		}
	}
}

void Session::ProcessConnect()
{
	mConnectEvent.mOwner = nullptr;	// Release

	mConnected.store(true);

	// Register Session
	GetService()->AddSession(GetSession());

	// Contents Code OverRiding
	OnConnected();

	// Register Recv
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	mDisconnectEvent.mOwner = nullptr;	// Release
}

void Session::ProcessRecv(int32 numOfBytes)
{
	mRecvEvent.mOwner = nullptr;	// Release

	if (numOfBytes == 0)
	{
		DisConnect("Recv 0");
		return;
	}

	if (mRecvBuffer.OnWrite(numOfBytes) == false)
	{
		DisConnect("Recv Buffer OverFlow");
		return;
	}

	int32 dataSize = mRecvBuffer.DataSize();
	int processLen = OnRecv(mRecvBuffer.ReadPos(), dataSize);
	if (processLen < 0 || dataSize < processLen || mRecvBuffer.OnRead(processLen) == false)
	{
		DisConnect("Recv Process Error");
		return;
	}

	// cusor 정리
	mRecvBuffer.Clean();

	// Register Recv
	RegisterRecv();
}

void Session::ProcessSend(SendEvent* sendEvent, int32 numOfBytes)
{
	sendEvent->mOwner = nullptr;	// Release
	delete(sendEvent);

	if (numOfBytes == 0)
	{
		DisConnect("Send 0");
		return;
	}

	// Contents Code OverRiding
	OnSend(numOfBytes);
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
