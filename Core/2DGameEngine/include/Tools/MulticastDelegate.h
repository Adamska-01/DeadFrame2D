#pragma once
#include <list>
#include <functional>


template<typename ... Args>
class MulticastDelegate
{
private:
	std::list<std::function<void(Args...)>> listeners;


public:
	MulticastDelegate() = default;

	~MulticastDelegate() = default;


	bool IsEmpty();


	MulticastDelegate& operator += (const std::function<void(Args...)> func);
	
	MulticastDelegate& operator -= (const std::function<void(Args...)> func);

	void operator() (Args... params);
};


using MultiCastVoid = MulticastDelegate<>;


template<typename ...Args>
inline bool MulticastDelegate<Args...>::IsEmpty()
{
	return listeners.empty();
}

template<typename ...Args>
inline MulticastDelegate<Args...>& MulticastDelegate<Args...>::operator+=(const std::function<void(Args...)> func)
{
	listeners.push_back(func);

	return *this;
}

template<typename ...Args>
inline MulticastDelegate<Args...>& MulticastDelegate<Args...>::operator-=(const std::function<void(Args...)> func)
{
	auto it = std::find_if(listeners.begin(), listeners.end(), [&func](const std::function<void(Args...)>& f) 
		{
			auto f_target = f.target<void(Args...)>();
			auto func_target = func.target<void(Args...)>();
			
			return f_target && func_target && f_target == func_target;
		});

	if (it != listeners.end())
	{
		listeners.erase(it);
	}

	return *this;
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::operator()(Args ...params)
{
	for (auto listener : listeners)
	{
		listener(params...);
	}
}