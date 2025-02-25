#pragma once

class SendBufferChunk;

class SendBuffer
{
public:
	SendBuffer(shared_ptr<SendBufferChunk> owner, byte* buffer, int32 allocSize);
	~SendBuffer();

	byte*	Buffer() { return mBuffer; }
	int32	WritePos() { return mWritePos; }
	void	Close(uint32 writeSize);


private:
	byte* mBuffer;
	uint32 mAllocSize = 0;
	uint32 mWritePos = 0;
	shared_ptr<SendBufferChunk> mOwner;
};