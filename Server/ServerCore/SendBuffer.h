#pragma once
class SendBuffer
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

	byte* Buffer() { return mBuffer.data(); }
	int32 WritePos() { return mWritePos; }
	int32 Capacity() { return static_cast<int32>(mBuffer.size()); }

	void CopyData(void* data, int32 len);


private:
	vector<byte> mBuffer;
	int32 mWritePos = 0;
};

