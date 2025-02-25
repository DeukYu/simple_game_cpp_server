#pragma once

class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
{
public:
	static constexpr uint32	CHUNK_SIZE = 0x1000;

public:
	SendBufferChunk();
	~SendBufferChunk();

	void Reset();
	shared_ptr<SendBuffer> Open(uint32 allocSize);
	void Close(int32 writeSize);

	bool IsOpen() { return mIsOpen; }
	byte* Buffer() { return &mBuffer[mUsedSize]; }
	uint32 FreeSize() { return static_cast<uint32>(mBuffer.size()) - mUsedSize; }

private:
	array<byte, CHUNK_SIZE> mBuffer;
	bool mIsOpen;
	uint32 mUsedSize;
};