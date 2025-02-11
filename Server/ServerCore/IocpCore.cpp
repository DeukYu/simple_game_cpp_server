#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore GIocpCore;

IocpCore::IocpCore()
{
	_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (_iocpHandle == NULL)
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
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	if (GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes, OUT &key,
		OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		shared_ptr<IocpObject> iocpObject = iocpEvent->m_owner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = WSAGetLastError();
		switch (errCode)
		{case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : ·Î±× Âï±â
			shared_ptr<IocpObject> iocpObject = iocpEvent->m_owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return false;
}
