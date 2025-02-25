#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(shared_ptr<SendBufferChunk> owner, byte* buffer, int32 allocSize)
	: mOwner(owner)	, mBuffer(buffer)	, mAllocSize(allocSize)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(uint32 writeSize)
{
	if (mAllocSize < writeSize)
	{
		// error
		return;
	}

	mWritePos = writeSize;
	mOwner->Close(writeSize);
}