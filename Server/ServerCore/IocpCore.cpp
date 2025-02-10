#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore GlobalCore;

IocpCore::IocpCore()
{
	_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (_iocpHandle != INVALID_HANDLE_VALUE)
	{
		throw runtime_error("Failed to create IOCP handle");
	}
}

IocpCore::~IocpCore()
{
	CloseHandle(_iocpHandle);
}

bool IocpCore::Register(IocpObject* iocpObject)
{
	return CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, reinterpret_cast<ULONG_PTR>(iocpObject), 0);
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	IocpObject* iocpObject = nullptr;
	IocpEvent* iocpEvent = nullptr;

	if (GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes, OUT reinterpret_cast<PULONG_PTR>(&iocpObject),
		OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = WSAGetLastError();
		switch (errCode)
		{case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : �α� ���
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return false;
}
