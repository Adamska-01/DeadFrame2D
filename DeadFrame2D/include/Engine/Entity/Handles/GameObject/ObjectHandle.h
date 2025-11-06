#pragma once
#include "Engine/Entity/Handles/GameObject/ObjectHandleBase.h"


namespace DeadFrame2D::Engine
{
	class ISceneHandleProvider;


	template<typename T>
	class ObjectHandle : public ObjectHandleBase
	{
		friend class Scene;


	private:
		T* Get() const;

		static ObjectHandle<T> ValidateAndReturnHandle(const ObjectHandleBase& base);


	public:
		ObjectHandle() = default;

		ObjectHandle(std::weak_ptr<ISceneHandleProvider> scene, uint32_t index, uint32_t generation);

		explicit ObjectHandle(const ObjectHandleBase& base);


		template<typename U>
		static ObjectHandle<T> From(const ObjectHandle<U>& other);

		static ObjectHandle<T> From(const ObjectHandleBase& base);


		T* operator->() const;

		T& operator*() const;

		template<typename U, std::enable_if_t<std::is_base_of_v<U, T>, int> = 0>
		operator ObjectHandle<U>() const;


		using ObjectHandleBase::operator=;
	};
}


#include "Engine/SceneSystem/ISceneHandleProvider.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Engine
{
	template<typename T>
	inline ObjectHandle<T>::ObjectHandle(std::weak_ptr<ISceneHandleProvider> scene, uint32_t index, uint32_t generation)
		: ObjectHandleBase(scene, index, generation)
	{
		static_assert(std::is_base_of_v<GameObject, T>, "T must derive from GameObject");
	}

	template<typename T>
	inline ObjectHandle<T>::ObjectHandle(const ObjectHandleBase& base)
		: ObjectHandleBase(base)
	{
	}


	template<typename T>
	inline T* ObjectHandle<T>::Get() const
	{
		auto locked = scene.lock();

		if (locked == nullptr || !IsValid())
			return nullptr;

		auto comp = locked->GetAt(index);

		return comp != nullptr ? static_cast<T*>(comp) : nullptr;
	}

	template<typename T>
	inline ObjectHandle<T> ObjectHandle<T>::ValidateAndReturnHandle(const ObjectHandleBase& base)
	{
		auto locked = base.scene.lock();

		if (!locked || !locked->IsValid(base.index, base.generation))
			return {};

		auto obj = locked->GetAt(base.index);

		return obj != nullptr ? ObjectHandle<T>(base) : ObjectHandle<T>();
	}

	template<typename T>
	template<typename U>
	inline ObjectHandle<T> ObjectHandle<T>::From(const ObjectHandle<U>& other)
	{
		static_assert(std::is_base_of_v<GameObject, U>, "U must derive from GameObject");
		static_assert(std::is_base_of_v<GameObject, T>, "T must derive from GameObject");
		static_assert(std::is_base_of_v<T, U>, "T must be a base of U (upcast only)");

		return ValidateAndReturnHandle(other);
	}

	template<typename T>
	inline ObjectHandle<T> ObjectHandle<T>::From(const ObjectHandleBase& base)
	{
		return ValidateAndReturnHandle(base);
	}

	template<typename T>
	inline T* ObjectHandle<T>::operator->() const
	{
		return Get();
	}

	template<typename T>
	inline T& ObjectHandle<T>::operator*() const
	{
		return *Get();
	}

	template<typename T>
	template<typename U, std::enable_if_t<std::is_base_of_v<U, T>, int>>
	inline ObjectHandle<T>::operator ObjectHandle<U>() const
	{
		return ObjectHandle<U>::From(*this);
	}
}