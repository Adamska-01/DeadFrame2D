#pragma once
#include "Core/Reflection/TypeInfoCheck.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentEntry.h"
#include <memory>
#include <vector>


namespace DF2D::Engine
{
	class GameObject;

	class GameComponent;

	template<typename T>
	class ComponentHandle;

	template<typename T>
	class ObjectHandle;


	class ComponentBucket : public std::enable_shared_from_this<ComponentBucket>
	{
		friend class Scene;

		friend class GameObject;

		template<typename T>
		friend class ComponentHandle;

		friend class ComponentHandleBase;


	private:
		std::vector<ComponentEntry> components;


		DF2D_API GameComponent* GetAt(uint32_t index) const;

		DF2D_API uint32_t FindFreeSlot();

		DF2D_API void LinkComponentToOwner(const ObjectHandle<GameObject>& owner, GameComponent* toInitialize);

		DF2D_API bool IsValid(uint32_t index, uint32_t generation) const;

		DF2D_API void SendGameComponentAddedEvent(ComponentHandle<GameComponent> newComponent) const;


		template<typename F> // Forward rvalue or lvalue without allocation
		void ForEach(F&& func);


	public:
		DF2D_API ComponentBucket();

		DF2D_API ~ComponentBucket();

		ComponentBucket(const ComponentBucket&) = delete;

		ComponentBucket& operator=(const ComponentBucket&) = delete;

		ComponentBucket(ComponentBucket&&) noexcept = default;

		ComponentBucket& operator=(ComponentBucket&&) noexcept = default;


		template<typename T>
		ComponentHandle<T> GetComponent();

		template<typename T, typename... Args>
		ComponentHandle<T> AddComponent(const ObjectHandle<GameObject>& owner, Args&&... args);

		template<typename T>
		void RemoveComponent(const ComponentHandle<T>& handle);
	};
}


#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"


namespace DF2D::Engine
{
	template<typename F>
	inline void ComponentBucket::ForEach(F&& func)
	{
		for (size_t i = 0; i < components.size(); ++i)
		{
			auto& entry = components[i];

			if (!entry.alive)
				continue;

			func(*entry.instance);
		}
	}


	template<typename T>
	inline ComponentHandle<T> ComponentBucket::GetComponent()
	{
		static_assert(std::is_base_of<GameComponent, T>::value, "T must derive from GameComponent");

		const auto* wantedType = &T::StaticTypeInfo;

		auto componentsSize = static_cast<uint32_t>(components.size());

		for (uint32_t i = 0; i < componentsSize; ++i)
		{
			const auto& entry = components[i];

			if (!entry.alive)
				continue;

			if (!entry.instance->IsA(wantedType))
				continue;

			return ComponentHandle<T>(weak_from_this(), i, entry.generation);
		}

		// No match found
		return {};
	}

	template<typename T, typename... Args>
	inline ComponentHandle<T> ComponentBucket::AddComponent(const ObjectHandle<GameObject>& owner, Args && ...args)
	{
		static_assert(std::is_base_of<GameComponent, T>::value, "GameComponent T must derive from GameComponent");
		static_assert(Core::HasTypeInfo<T>::value, "GameComponent T must declare TYPE_INFO");

		auto index = FindFreeSlot();

		auto& entry = components[index];

		entry.instance = std::make_unique<T>(std::forward<Args>(args)...);
		entry.alive = true;

		auto handle = ComponentHandle<T>(weak_from_this(), index, entry.generation);

		entry.instance.get()->selfHandle = handle;

		if (owner != nullptr)
		{
			LinkComponentToOwner(owner, entry.instance.get());
		}

		SendGameComponentAddedEvent(ComponentHandle<GameComponent>::From(handle));

		return handle;
	}

	template<typename T>
	inline void ComponentBucket::RemoveComponent(const ComponentHandle<T>& handle)
	{
		if (handle.GetBucket() != this)
			return;

		if (!IsValid(handle.GetIndex(), handle.GetGeneration()))
			return;

		auto& entry = components[handle.GetIndex()];

		entry.instance.reset();
		entry.alive = false;
		entry.generation++;
	}
}