
#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

struct TestObject
{
	int x;
	TestObject() {}
	TestObject(int value) : x(value) { std::cout << "Constructed: " << x << "\n"; }
	~TestObject() { std::cout << "Destructed: " << x << "\n"; }
};

void SingleThreadTest()
{
	std::cout << "=== Single Thread Test ===\n";

	TestObject* obj1 = ObjectPool<TestObject>::Pop(10);
	std::cout << "Value: " << obj1->x << "\n";

	ObjectPool<TestObject>::Push(obj1);

	auto sharedObj = ObjectPool<TestObject>::MakeShared();
	sharedObj->x = 20;
	std::cout << "Shared Value: " << sharedObj->x << "\n";
}

void MultiThreadTest()
{
	std::cout << "=== Multi Thread Test ===\n";

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]() {
			TestObject* obj = ObjectPool<TestObject>::Pop(i);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			ObjectPool<TestObject>::Push(obj);
			});
	}
	GThreadManager->Join();
}

void ReuseTest()
{
	std::cout << "=== Reuse Test ===\n";

	auto obj1 = ObjectPool<TestObject>::Pop(30);
	auto obj2 = ObjectPool<TestObject>::Pop(40);

	ObjectPool<TestObject>::Push(obj1);
	ObjectPool<TestObject>::Push(obj2);

	auto obj3 = ObjectPool<TestObject>::Pop(50); // Should reuse obj1's memory
	std::cout << "Reused Value: " << obj3->x << "\n";

	ObjectPool<TestObject>::Push(obj3);
}

int main()
{

	SingleThreadTest();

	MultiThreadTest();

	ReuseTest();
}