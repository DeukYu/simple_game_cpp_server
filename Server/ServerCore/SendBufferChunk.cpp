#include "pch.h"
#include "SendBufferChunk.h"

SendBufferChunk::SendBufferChunk()
	: mBuffer(), mIsOpen(false), mUsedSize(0)
{
}

SendBufferChunk::~SendBufferChunk()
{
}

void SendBufferChunk::Reset()
{
	mIsOpen = false;
	mUsedSize = 0;
}

shared_ptr<SendBuffer> SendBufferChunk::Open(uint32 allocSize)
{
	if (allocSize > CHUNK_SIZE || mIsOpen)
	{
		// error
		return nullptr;
	}

	if (allocSize > FreeSize())
	{
		return nullptr;
	}

	mIsOpen = true;
	return make_shared<SendBuffer>(shared_from_this(), Buffer(), allocSize);
}

void SendBufferChunk::Close(int32 writeSize)
{
	if (!mIsOpen || writeSize > FreeSize())
	{
		// error
		return;
	}

	mUsedSize += writeSize;
	mIsOpen = false;
}
