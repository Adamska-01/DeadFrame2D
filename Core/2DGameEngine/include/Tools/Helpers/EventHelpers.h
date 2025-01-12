#pragma once
#include <functional>


namespace EventHelpers
{
	template <typename T, typename R, typename... Args>
	std::function<R(Args...)> BindFunction(T* instance, R(T::* member_function)(Args...)) 
	{
		return [instance, member_function](Args... args) 
			{
				return (instance->*member_function)(std::forward<Args>(args)...);
			};
	}
}