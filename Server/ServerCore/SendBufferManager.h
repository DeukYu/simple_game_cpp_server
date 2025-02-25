#pragma once
class SendBufferManager
{
public:
	shared_ptr<SendBuffer> Open(int32 size);

private:
	shared_ptr<SendBufferChunk> Pop();
	void Push(shared_ptr<SendBufferChunk> buffer);

	static void PushGlobal(SendBufferChunk* buffer);

private:
	mutex mLock;
	vector<shared_ptr<SendBufferChunk>> mSendBufferChunks;
};