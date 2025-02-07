#pragma once

template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
		void* memory = s_resource.allocate(s_allocSize);
		new(memory) Type(forward<Args>(args)...);
		return static_cast<Type*>(memory);
	}

	static void Push(Type* obj)
	{
		if (!obj)
			return;

		obj->~Type();
		s_resource.deallocate(obj, s_allocSize);
	}

	static shared_ptr<Type> MakeShared()
	{
		shared_ptr<Type> ptr(Pop(), [](Type* obj) { Push(obj); });
		return ptr;
	}

private:
	static inline pmr::synchronized_pool_resource s_resource{};
	static inline size_t s_allocSize = sizeof(Type);
};