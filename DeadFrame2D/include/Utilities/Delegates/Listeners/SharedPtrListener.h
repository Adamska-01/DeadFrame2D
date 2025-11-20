#pragma once
#include "Utilities/Delegates/Listeners/Abstractions/IListener.h"
#include <memory>


namespace DeadFrame2D::Utilities
{
	template<typename T, typename... Args>
	struct SharedPtrListener : IListener<Args...>
	{
		std::weak_ptr<T> weakOwner;
		
		void (T::* memFn)(Args...);


		SharedPtrListener(std::weak_ptr<T> w, void (T::* m)(Args...));


		void Invoke(const Args& ...args) override;

		bool IsExpired() const override;

		bool MatchesIdentity(const void* identity, const std::type_info& type) const override;
	};
}


namespace DeadFrame2D::Utilities
{
	template<typename T, typename... Args>
	inline SharedPtrListener<T, Args...>::SharedPtrListener(std::weak_ptr<T> w, void(T::* m)(Args...))
		: weakOwner(std::move(w)), memFn(m)
	{
	}

	template<typename T, typename... Args>
	inline void SharedPtrListener<T, Args...>::Invoke(const Args& ...args)
	{
		auto sPtr = weakOwner.lock();

		if (!sPtr)
			return;

		(sPtr.get()->*memFn)(args...);
	}

	template<typename T, typename... Args>
	inline bool SharedPtrListener<T, Args...>::IsExpired() const
	{
		return weakOwner.expired();
	}

	template<typename T, typename... Args>
	bool SharedPtrListener<T, Args...>::MatchesIdentity(const void* identity, const std::type_info& type) const
	{
		if (type != typeid(std::shared_ptr<T>))
			return false;

		auto owner = weakOwner.lock();

		if (!owner)
			return false;

		auto* sp = static_cast<const std::shared_ptr<T>*>(identity);

		return owner.get() == sp->get();
	}
}