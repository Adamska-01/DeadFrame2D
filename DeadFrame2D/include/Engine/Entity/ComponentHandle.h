#pragma once
#include "Engine/Entity/ComponentHandleBase.h"
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
		bool IsValid() const;

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

		bool operator==(const ComponentHandle<T>& other) const;

		bool operator!=(const ComponentHandle<T>& other) const;

		bool operator==(std::nullptr_t) const;

		bool operator!=(std::nullptr_t) const;

		ComponentHandle<T>& operator=(std::nullptr_t);

		template<typename U, typename = typename std::enable_if<std::is_base_of<U, T>::value>::type>
		operator ComponentHandle<U>() const;

		explicit operator bool() const;
	};
}


#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/ComponentBucket.h"


namespace DeadFrame2D::Engine
{
	template<typename T>
	inline ComponentHandle<T>::ComponentHandle(std::weak_ptr<ComponentBucket> bucket, uint32_t index, uint32_t generation)
		: ComponentHandleBase(std::move(bucket), index, generation)
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
		auto locked = base.bucket.lock();

		if (!locked)
			return {};

		if (!locked->IsAlive(base.index, base.generation))
			return {};

		auto comp = locked->GetAt(base.index);
		
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
	inline bool ComponentHandle<T>::IsValid() const
	{
		auto locked = bucket.lock();

		return locked != nullptr ? locked->IsAlive(index, generation) : false;
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
	inline bool ComponentHandle<T>::operator==(const ComponentHandle<T>& other) const
	{
		return bucket.lock() == other.bucket.lock()
			&& index == other.index
			&& generation == other.generation;
	}

	template<typename T>
	inline bool ComponentHandle<T>::operator!=(const ComponentHandle<T>& other) const
	{
		return !(*this == other);
	}

	template<typename T>
	inline bool ComponentHandle<T>::operator==(std::nullptr_t) const
	{
		return !IsValid();
	}

	template<typename T>
	inline bool ComponentHandle<T>::operator!=(std::nullptr_t) const
	{
		return IsValid();
	}

	template<typename T>
	inline ComponentHandle<T>& ComponentHandle<T>::operator=(std::nullptr_t)
	{
		bucket.reset();
		index = UINT32_MAX;
		generation = 0;

		return *this;
	}

	template<typename T>
	template<typename U, typename>
	inline ComponentHandle<T>::operator ComponentHandle<U>() const
	{
		static_assert(std::is_base_of<U, T>::value, "U must be a base class of T");

		return ComponentHandle<U>::From(*this);
	}

	template<typename T>
	inline ComponentHandle<T>::operator bool() const
	{
		return IsValid();
	}
}