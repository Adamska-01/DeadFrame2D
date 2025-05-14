#pragma once
#include <functional>
#include <unordered_map>


template<typename ... Args>
class MulticastDelegate
{
private:
	// TODO: it should be a list of std::function paired with their identifier.
	std::unordered_map<std::uintptr_t, std::function<void(Args...)>> listeners;


public:
	MulticastDelegate() = default;

	~MulticastDelegate() = default;


	bool IsEmpty();

	void Clear();


	void RegisterCallback(const std::function<void(Args...)>& func, std::uintptr_t identifier);

	void DeregisterCallback(std::uintptr_t identifier);


	void operator() (Args... params);
};


using MultiCastVoid = MulticastDelegate<>;


template<typename ...Args>
inline bool MulticastDelegate<Args...>::IsEmpty()
{
	return listeners.empty();
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::Clear()
{
	listeners.clear();
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::RegisterCallback(const std::function<void(Args...)>& func, std::uintptr_t identifier)
{
	listeners[identifier] = func;
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::DeregisterCallback(std::uintptr_t identifier)
{
	listeners.erase(identifier);
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::operator()(Args ...params)
{
	for (const auto& [_, listener] : listeners)
	{
		listener(params...);
	}
}