#include "pch.h"
#include "SendBufferManager.h"

shared_ptr<SendBuffer> SendBufferManager::Open(int32 size)
{
	if (LSendBufferChunk == nullptr)
	{
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	if (LSendBufferChunk->IsOpen())
	{
		// error
		return nullptr;
	}

	if (LSendBufferChunk->FreeSize() < size)
	{
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	return LSendBufferChunk->Open(size);
}

shared_ptr<SendBufferChunk> SendBufferManager::Pop()
{
	{
		unique_lock lock(mLock);
		if (mSendBufferChunks.empty() == false)
		{
			auto sendBufferChunk = mSendBufferChunks.back();
			mSendBufferChunks.pop_back();
			return sendBufferChunk;
		}
	}

	return shared_ptr<SendBufferChunk>(new SendBufferChunk(), PushGlobal);
}

void SendBufferManager::Push(shared_ptr<SendBufferChunk> buffer)
{
	unique_lock lock(mLock);
	mSendBufferChunks.push_back(buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* buffer)
{
	GSendBufferManager->Push(shared_ptr<SendBufferChunk>(buffer, PushGlobal));
}