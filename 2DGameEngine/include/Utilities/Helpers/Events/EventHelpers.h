#pragma once
#include <functional>


namespace DeadFrame2D::Utilities
{
	// TODO: Put this in a statci class
	template <typename T, typename R, typename... Args>
	inline std::function<R(Args...)> BindFunction(T* instance, R(T::* member_function)(Args...))
	{
		return [instance, member_function](Args... args) 
			{
				return (instance->*member_function)(std::forward<Args>(args)...);
			};
	}
}