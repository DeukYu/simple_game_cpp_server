#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(int32 bufferSize)
{
	mBuffer.resize(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* data, int32 len)
{
	if(Capacity() < len)
	{
		return;
	}

	memcpy(mBuffer.data(), data, len);
	mWritePos = len;
}
