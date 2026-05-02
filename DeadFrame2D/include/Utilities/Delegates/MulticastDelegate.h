#pragma once
#include "Utilities/Delegates/Listeners/Abstractions/IListener.h"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>
#include <memory>


namespace DF2D::Engine
{
	class ObjectHandleBase;
	class ComponentHandleBase;
}


namespace DF2D::Utilities
{
	template<typename ... Args>
	class MulticastDelegate
	{
	private:
		std::vector<std::unique_ptr<IListener<Args...>>> listeners;

		ListenerID nextID = 0;


		void PruneExpired();


	public:
		MulticastDelegate() = default;

		~MulticastDelegate() = default;

		MulticastDelegate(const MulticastDelegate&) = delete;

		MulticastDelegate& operator=(const MulticastDelegate&) = delete;

		MulticastDelegate(MulticastDelegate&&) noexcept = default;

		MulticastDelegate& operator=(MulticastDelegate&&) noexcept = default;


		template<typename T>
		ListenerID AddRaw(T* instance, void (T::* func)(Args...));

		template<typename T>
		ListenerID AddShared(const std::shared_ptr<T>& instance, void (T::* func)(Args...));

		/**
		* Adds a functor. This is generally used for lambda functions that do not capture pointers.
		*/
		ListenerID AddLambda(std::function<void(Args...)> func);

		ListenerID AddHandle(const Engine::ObjectHandleBase& handle, std::function<void(Args...)> func);

		ListenerID AddHandle(const Engine::ComponentHandleBase& handle, std::function<void(Args...)> func);


		/**
		 * @brief Invokes all active listeners.
		 */
		void Broadcast(const Args& ...args);

		/**
		 * @brief Removes a specific listener by comparing identity (e.g., owner pointer).
		 */
		template<typename T>
		void RemoveByListener(const T* listener);

		/**
		 * @brief Removes a listener with a specific ID.
		 */
		void RemoveByID(ListenerID id);

		/**
		 * @brief Clears the entire delegate list.
		 */
		void Clear();

		/**
		 * @brief Returns number of active (non-expired) listeners.
		 */
		size_t Count() const;

		/**
		 * @brief Returns true if there are no listeners.
		 */
		bool IsEmpty() const;
	};


	using MultiCastVoid = MulticastDelegate<>;
}


#include "Utilities/Delegates/Listeners/HandleListener.h"
#include "Utilities/Delegates/Listeners/LambdaListener.h"
#include "Utilities/Delegates/Listeners/RawListener.h"
#include "Utilities/Delegates/Listeners/SharedPtrListener.h"


namespace DF2D::Utilities
{
	template<typename ...Args>
	inline void MulticastDelegate<Args...>::PruneExpired()
	{
		listeners.erase(
			std::remove_if(
				listeners.begin(),
				listeners.end(),
				[](const std::unique_ptr<IListener<Args...>>& listener)
				{
					return listener->IsExpired();
				}),
			listeners.end());
	}


	template<typename ...Args>
	template<typename T>
	inline ListenerID MulticastDelegate<Args...>::AddRaw(T* instance, void(T::* func)(Args...))
	{
		auto id = nextID++;

		auto listener = std::make_unique<RawListener<T, Args...>>(instance, func);
		listener->SetID(id);
		
		listeners.push_back(std::move(listener));

		return id;
	}

	template<typename ...Args>
	template<typename T>
	inline ListenerID MulticastDelegate<Args...>::AddShared(const std::shared_ptr<T>& instance, void(T::* func)(Args...))
	{
		auto id = nextID++;
		
		auto listener = std::make_unique<SharedPtrListener<T, Args...>>(instance, func);
		listener->SetID(id);

		listeners.push_back(std::move(listener));

		return id;
	}

	template<typename ...Args>
	inline ListenerID MulticastDelegate<Args...>::AddLambda(std::function<void(Args...)> func)
	{
		auto id = nextID++;
		
		auto listener = std::make_unique<LambdaListener<Args...>>(std::move(func));
		listener->SetID(id);

		listeners.push_back(std::move(listener));

		return id;
	}

	template<typename ...Args>
	inline ListenerID MulticastDelegate<Args...>::AddHandle(const Engine::ObjectHandleBase& handle, std::function<void(Args...)> func)
	{
		auto id = nextID++;

		auto listener = std::make_unique<HandleListener<Args...>>(handle, std::move(func));
		listener->SetID(id);

		listeners.push_back(std::move(listener));

		return id;
	}

	template<typename ...Args>
	inline ListenerID MulticastDelegate<Args...>::AddHandle(const Engine::ComponentHandleBase& handle, std::function<void(Args...)> func)
	{
		auto id = nextID++;

		auto listener = std::make_unique<HandleListener<Args...>>(handle, std::move(func));
		listener->SetID(id);

		listeners.push_back(std::move(listener));

		return id;
	}

	template<typename ...Args>
	inline void MulticastDelegate<Args...>::Broadcast(const Args& ...args)
	{
		PruneExpired();

		for (auto& listener : listeners)
		{
			listener->Invoke(args...);
		}
	}

	template<typename ...Args>
	template<typename T>
	inline void MulticastDelegate<Args...>::RemoveByListener(const T* listener)
	{
		listeners.erase(
			std::remove_if(
				listeners.begin(), 
				listeners.end(),
				[&](const auto& l) 
				{
					return l->Matches(*listener);
				}),
			listeners.end());
	}

	template<typename ...Args>
	inline void MulticastDelegate<Args...>::RemoveByID(ListenerID id)
	{
		listeners.erase(
			std::remove_if(
				listeners.begin(), 
				listeners.end(),
				[id](const auto& l)
				{
					return l->GetID() == id;
				}),
			listeners.end());
	}

	template<typename ...Args>
	void MulticastDelegate<Args...>::Clear()
	{
		listeners.clear();
	}

	template<typename ...Args>
	inline size_t MulticastDelegate<Args...>::Count() const
	{
		return std::count_if(
			listeners.begin(),
			listeners.end(),
			[](const auto& l)
			{
				return !l->IsExpired();
			});
	}

	template<typename ...Args>
	inline bool MulticastDelegate<Args...>::IsEmpty() const
	{
		return Count() == 0;
	}
}