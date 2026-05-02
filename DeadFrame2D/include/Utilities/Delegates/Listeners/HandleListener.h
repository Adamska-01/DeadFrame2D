#pragma once
#include "Engine/ECS/Entity/Component/Handle/ComponentHandleBase.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandleBase.h"
#include "Utilities/Delegates/Listeners/Abstractions/IListener.h"
#include <functional>
#include <variant>


namespace DF2D::Utilities
{
	/**
	 * @brief Listener bound to an engine handle (ObjectHandleBase or ComponentHandleBase).
	 * It automatically validates the handle before invocation and skips expired ones.
	 */
	template<typename... Args>
	struct HandleListener final : IListener<Args...>
	{
		std::variant<
			Engine::ComponentHandleBase,
			Engine::ObjectHandleBase
		> handle;

		std::function<void(Args...)> func;


		HandleListener(const Engine::ComponentHandleBase& compHandle, std::function<void(Args...)> f);

		HandleListener(const Engine::ObjectHandleBase& objHandle, std::function<void(Args...)> f);


		void Invoke(const Args& ...args) override;

		bool IsExpired() const override;

		bool MatchesIdentity(const void* identity, const std::type_info& type) const override;
	};
}


namespace DF2D::Utilities
{
	template<typename ...Args>
	inline HandleListener<Args...>::HandleListener(const Engine::ComponentHandleBase& compHandle, std::function<void(Args...)> f)
		: handle(compHandle), func(std::move(f))
	{
	}

	template<typename ...Args>
	inline HandleListener<Args...>::HandleListener(const Engine::ObjectHandleBase& objHandle, std::function<void(Args...)> f)
		: handle(objHandle), func(std::move(f)) 
	{
	}


	template<typename ...Args>
	inline void HandleListener<Args...>::Invoke(const Args& ...args)
	{
		std::visit([&](auto&& h)
			{
				if (h == nullptr)
					return;

				func(args...);
			}, 
			handle);
	}

	template<typename ...Args>
	inline bool HandleListener<Args...>::IsExpired() const
	{
		auto expired = true;
		
		std::visit([&](auto&& h)
			{
				expired = (h == nullptr);
			}, 
			handle);

		return expired;
	}

	template<typename ...Args>
	inline bool HandleListener<Args...>::MatchesIdentity(const void* identity, const std::type_info& type) const
	{
		using namespace DF2D::Engine;


		if (type == typeid(ObjectHandleBase))
		{
			const auto& given = *static_cast<const ObjectHandleBase*>(identity);

			if (std::holds_alternative<ObjectHandleBase>(handle))
				return std::get<ObjectHandleBase>(handle) == given;

			return false;
		}

		if (type == typeid(ComponentHandleBase))
		{
			const auto& given = *static_cast<const ComponentHandleBase*>(identity);

			if (std::holds_alternative<ComponentHandleBase>(handle))
				return std::get<ComponentHandleBase>(handle) == given;

			return false;
		}

		return false;
	}
}