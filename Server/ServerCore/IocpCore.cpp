#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore::IocpCore()
{
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mIocpHandle == NULL)
	{
		throw runtime_error("Failed to create IOCP handle");
	}
}

IocpCore::~IocpCore()
{
	CloseHandle(mIocpHandle);
}

bool IocpCore::Register(shared_ptr<IocpObject> iocpObject)
{
	return CreateIoCompletionPort(iocpObject->GetHandle(), mIocpHandle, 0, 0);
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	if (GetQueuedCompletionStatus(mIocpHandle, OUT & numOfBytes, OUT &key,
		OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		shared_ptr<IocpObject> iocpObject = iocpEvent->mOwner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = WSAGetLastError();
		switch (errCode)
		{case WAIT_TIMEOUT:
			return false;
		default:
			shared_ptr<IocpObject> iocpObject = iocpEvent->mOwner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}
