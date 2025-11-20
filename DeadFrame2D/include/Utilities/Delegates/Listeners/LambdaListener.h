#pragma once
#include "Utilities/Delegates/Listeners/Abstractions/IListener.h"
#include <functional>


namespace DeadFrame2D::Utilities
{
	template<typename... Args>
	struct LambdaListener final : IListener<Args...>
	{
		std::function<void(Args...)> func;


		explicit LambdaListener(std::function<void(Args...)> f);


		void Invoke(const Args& ...args) override;

		bool IsExpired() const override;

		bool MatchesIdentity(const void* identity, const std::type_info& type) const override;
	};
}


namespace DeadFrame2D::Utilities
{
	template<typename ...Args>
	inline LambdaListener<Args...>::LambdaListener(std::function<void(Args...)> f)
		: func(std::move(f))
	{
	}

	template<typename ...Args>
	inline void LambdaListener<Args...>::Invoke(const Args& ...args)
	{
		func(args...);
	}

	template<typename ...Args>
	inline bool LambdaListener<Args...>::IsExpired() const
	{
		return false;
	}

	template<typename ...Args>
	inline bool DeadFrame2D::Utilities::LambdaListener<Args...>::MatchesIdentity(const void* identity, const std::type_info& type) const
	{
		// Lambda has no owner, so it never matches anything
		return false;
	}
}