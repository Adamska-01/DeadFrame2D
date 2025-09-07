#pragma once
#include <memory>


template<typename T>
struct WeakPtrHash 
{
	size_t operator()(const std::weak_ptr<T>& wp) const 
	{
		auto sp = wp.lock();
	
		return std::hash<T*>()(sp.get());
	}
};