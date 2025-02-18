#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int32 bufferSize)
	:mCapacity(bufferSize* BUFFER_COUNT), mBufferSize(bufferSize)
{
	mBuffer.resize(mCapacity);
}

void RecvBuffer::Clean()
{
	if (int32 dataSize = DataSize(); dataSize == 0)
	{
		mReadPos = 0;
		mWritePos = 0;
	}
	else
	{
		if (FreeSize() < mBufferSize)
		{
			memcpy(&mBuffer[0], &mBuffer[mReadPos], dataSize);
			mReadPos = 0;
			mWritePos = dataSize;
		}
	}
}

bool RecvBuffer::OnRead(int32 numOfBytes)
{
	if (numOfBytes > DataSize())
		return false;

	mReadPos += numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 numOfBytes)
{
	if (numOfBytes > FreeSize())
		return false;

	mWritePos += numOfBytes;
	return true;
}
