#pragma once
#include "Engine/ECS/Entity/Component/Handle/ComponentHandleBase.h"
#include <cstdint>
#include <memory>
#include <type_traits>


namespace DeadFrame2D::Engine
{
	class ComponentBucket;


	template<typename T>
	class ComponentHandle : public ComponentHandleBase
	{
	private:
		T* Get() const;

		static ComponentHandle<T> ValidateAndReturnHandle(const ComponentHandleBase& base);


	public:
		ComponentHandle() = default;

		ComponentHandle(std::weak_ptr<ComponentBucket> bucket, uint32_t index, uint32_t generation);

		explicit ComponentHandle(const ComponentHandleBase& base);


		template<typename U>
		static ComponentHandle<T> From(const ComponentHandle<U>& other);

		static ComponentHandle<T> From(const ComponentHandleBase& base);


		T* operator->() const;

		T& operator*()  const;

		T* operator()() const;

		template<typename U, typename = typename std::enable_if<std::is_base_of<U, T>::value>::type>
		operator ComponentHandle<U>() const;


		using ComponentHandleBase::operator=;
	};
}


#include "Engine/ECS/Component/GameComponent.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"


namespace DeadFrame2D::Engine
{
	template<typename T>
	inline ComponentHandle<T>::ComponentHandle(std::weak_ptr<ComponentBucket> bucket, uint32_t index, uint32_t generation)
		: ComponentHandleBase(bucket, index, generation)
	{
		static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");
	}

	template<typename T>
	inline ComponentHandle<T>::ComponentHandle(const ComponentHandleBase& base)
		: ComponentHandleBase(base)
	{
	}


	template<typename T>
	inline ComponentHandle<T> ComponentHandle<T>::ValidateAndReturnHandle(const ComponentHandleBase& base)
	{
		auto baseBucket = base.GetBucket();

		if (baseBucket == nullptr || !baseBucket->IsValid(base.GetIndex(), base.GetGeneration()))
			return {};

		auto comp = baseBucket->GetAt(base.GetIndex());

		if (!comp || !comp->IsA(&T::StaticTypeInfo))
			return {};

		return ComponentHandle<T>(base);
	}


	template<typename T>
	template<typename U>
	inline ComponentHandle<T> ComponentHandle<T>::From(const ComponentHandle<U>& other)
	{
		static_assert(std::is_base_of_v<GameComponent, U>, "U must derive from GameComponent");
		static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");
		static_assert(std::is_base_of_v<T, U>, "T must be a base of U (upcast only)");

		return ValidateAndReturnHandle(other);
	}

	template<typename T>
	inline ComponentHandle<T> ComponentHandle<T>::From(const ComponentHandleBase& base)
	{
		return ValidateAndReturnHandle(base);
	}


	template<typename T>
	inline T* ComponentHandle<T>::Get() const
	{
		auto locked = bucket.lock();

		if (locked == nullptr)
			return nullptr;

		auto comp = locked->GetAt(index);

		return (IsValid() && comp->IsA(&T::StaticTypeInfo))
			? static_cast<T*>(comp)
			: nullptr;
	}


	template<typename T>
	inline T* ComponentHandle<T>::operator->() const
	{
		return Get();
	}

	template<typename T>
	inline T& ComponentHandle<T>::operator*() const
	{
		return *Get();
	}

	template<typename T>
	inline T* ComponentHandle<T>::operator()() const
	{
		return Get();
	}

	template<typename T>
	template<typename U, typename>
	inline ComponentHandle<T>::operator ComponentHandle<U>() const
	{
		return ComponentHandle<U>::From(*this);
	}
}