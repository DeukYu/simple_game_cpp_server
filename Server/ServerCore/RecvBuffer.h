#pragma once

// Circular Buffer

class RecvBuffer
{
	enum {
		BUFFER_COUNT = 10,
	};

public:
	explicit RecvBuffer(int32 bufferSize);
	~RecvBuffer() = default;

	void Clean();
	bool OnRead(int32 numOfBytes);
	bool OnWrite(int32 numOfBytes);

	[[nodiscard]] byte* ReadPos() { return &mBuffer[mReadPos]; }
	[[nodiscard]] byte* WritePos() { return &mBuffer[mWritePos]; }
	[[nodiscard]] int32 DataSize() const { return mWritePos - mReadPos; }
	[[nodiscard]] int32 FreeSize() const { return mCapacity - mWritePos; }


private:
	const int32 mCapacity;
	const int32 mBufferSize;
	int32 mReadPos = 0;
	int32 mWritePos = 0;
	vector<byte> mBuffer;
};

