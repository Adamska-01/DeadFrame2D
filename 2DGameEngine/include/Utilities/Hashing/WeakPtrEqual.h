#pragma once
#include <memory>


template<typename T>
struct WeakPtrEqual 
{
	bool operator()(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>& rhs) const 
	{
		return lhs.lock() == rhs.lock();
	}
};