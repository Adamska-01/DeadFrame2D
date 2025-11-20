#pragma once
#include "Utilities/Delegates/Listeners/Abstractions/IListener.h"


namespace DeadFrame2D::Utilities
{
	template<typename T, typename... Args>
	struct RawListener : IListener<Args...>
	{
		T* owner;

		void (T::* memFn)(Args...);


		RawListener(T* o, void (T::* m)(Args...));


		void Invoke(const Args& ...args) override;

		bool IsExpired() const override;

		bool MatchesIdentity(const void* identity, const std::type_info& type) const override;
	};
}


namespace DeadFrame2D::Utilities
{
	template<typename T, typename ...Args>
	inline RawListener<T, Args...>::RawListener(T* o, void(T::* m)(Args...))
		: owner(o), memFn(m) 
	{
	}

	template<typename T, typename ...Args>
	inline void RawListener<T, Args...>::Invoke(const Args& ...args)
	{
		(owner->*memFn)(args...);
	}

	template<typename T, typename ...Args>
	inline bool RawListener<T, Args...>::IsExpired() const
	{
		// Unsafe (But that's the whole point of raw listeners)
		return owner == nullptr;
	}

	template<typename T, typename... Args>
	bool RawListener<T, Args...>::MatchesIdentity(const void* identity, const std::type_info& type) const
	{
		if (type != typeid(T))
			return false;

		auto* ptr = static_cast<const T*>(identity);

		return owner == ptr;
	}
}