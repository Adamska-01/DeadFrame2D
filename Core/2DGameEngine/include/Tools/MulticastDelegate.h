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


	template<typename U>
	MulticastDelegate& operator += (const U& func);

	MulticastDelegate& operator += (const std::function<void(Args...)> func);
	
	template<typename U>
	MulticastDelegate& operator -= (const U& func);
	
	MulticastDelegate& operator -= (const std::function<void(Args...)> func);

	void operator() (Args... params);
};


template<typename ...Args>
template<typename U>
inline MulticastDelegate<Args...>& MulticastDelegate<Args...>::operator+=(const U& func)
{
	listeners.push_back(std::function<void(Args...)>(func));

	return *this;
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
	auto it = std::find(listeners.begin(), listeners.end(), func);

	if (it != listeners.end())
	{
		listeners.erase(it);
	}

	return *this;
}

template<typename ...Args>
template<typename U>
inline MulticastDelegate<Args...>& MulticastDelegate<Args...>::operator-=(const U& func)
{
	auto it = std::find(listeners.begin(), listeners.end(), func);

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