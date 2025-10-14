#pragma once
#include "Core/Reflection/TypeInfoCheck.h"
#include "Engine/Entity/ComponentEntry.h"
#include <cassert>
#include <memory>
#include <vector>


namespace DeadFrame2D::Engine
{
	class GameComponent;
	class GameObject;

	template<typename T> 
	class ComponentHandle;


	class ComponentBucket : public std::enable_shared_from_this<ComponentBucket>
	{
		friend class GameObject;

		friend class GameComponent;

		template<typename T>
		friend class ComponentHandle;


	private:
		std::vector<ComponentEntry> components;


		GameComponent* GetAt(uint32_t index) const;

		uint32_t FindFreeSlot();

		void LinkComponentToOwner(std::weak_ptr<GameObject> owner, GameComponent* toInitialize);

		bool IsAlive(uint32_t index, uint32_t generation) const;


		template<typename F> // Forward rvalue or lvalue without allocation
		void ForEach(F&& func);


	public:
		ComponentBucket();

		~ComponentBucket();

		ComponentBucket(const ComponentBucket&) = delete;

		ComponentBucket& operator=(const ComponentBucket&) = delete;

		ComponentBucket(ComponentBucket&&) noexcept = default;

		ComponentBucket& operator=(ComponentBucket&&) noexcept = default;


		template<typename T>
		ComponentHandle<T> GetComponent();

		template<typename T, typename... Args>
		ComponentHandle<T> AddComponent(std::weak_ptr<GameObject> owner, bool canInitialize, Args&&... args);

		template<typename T>
		void RemoveComponent(const ComponentHandle<T>& handle);
	};
}


#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Engine
{
	inline ComponentBucket::ComponentBucket()
	{
		components.clear();
	}

	inline ComponentBucket::~ComponentBucket()
	{
		components.clear();
	}


	inline GameComponent* ComponentBucket::GetAt(uint32_t index) const
	{
		assert(index < components.size());

		return components[index].instance.get();
	}

	inline uint32_t ComponentBucket::FindFreeSlot()
	{
		for (uint32_t i = 0; i < components.size(); ++i)
		{
			if (!components[i].alive)
				return i;
		}

		components.emplace_back();

		return static_cast<uint32_t>(components.size() - 1);
	}

	inline void ComponentBucket::LinkComponentToOwner(std::weak_ptr<GameObject> owner, GameComponent* toInitialize)
	{
		toInitialize->OwningObject = owner;

		toInitialize->RegisterAllHandlers(owner);
	}

	inline bool ComponentBucket::IsAlive(uint32_t index, uint32_t generation) const
	{
		if (index >= components.size()) 
			return false;

		const auto& entry = components[index];

		return entry.alive && entry.generation == generation;
	}

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

		for (size_t i = 0; i < components.size(); ++i)
		{
			const auto& entry = components[i];

			if (!entry.alive)
				continue;

			auto ptr = dynamic_cast<T*>(entry.instance.get());

			if (ptr != nullptr)
				return ComponentHandle<T>((weak_from_this()), static_cast<uint32_t>(i), entry.generation);
		}

		// No match found
		return ComponentHandle<T>();
	}

	template<typename T, typename ...Args>
	inline ComponentHandle<T> ComponentBucket::AddComponent(std::weak_ptr<GameObject> owner, bool canInitialize, Args && ...args)
	{
		static_assert(std::is_base_of<GameComponent, T>::value, "GameComponent T must derive from GameComponent");
		static_assert(DeadFrame2D::Core::HasTypeInfo<T>::value, "GameComponent T must declare TYPE_INFO");

		auto index = FindFreeSlot();

		auto& entry = components[index];

		entry.instance = std::make_unique<T>(std::forward<Args>(args)...);
		entry.alive = true;

		// This is necessary due to smart pointer/C++ limitations
		if (owner.lock() != nullptr)
		{
			LinkComponentToOwner(owner, entry.instance.get());
		}

		if (canInitialize)
		{
			LinkComponentToOwner(owner, entry.instance.get());
			
			entry.instance->Init();
			entry.instance->Start();
		}

		auto handle = ComponentHandle<T>(weak_from_this(), index, entry.generation);

		entry.instance.get()->selfHandle = handle;

		return handle;
	}

	template<typename T>
	inline void ComponentBucket::RemoveComponent(const ComponentHandle<T>& handle)
	{
		auto locked = handle.bucket.lock();

		if (locked == nullptr)
			return;

		if (!locked->IsAlive(handle.index, handle.generation))
			return;

		auto& entry = locked->components[handle.index];

		entry.instance.reset();
		entry.alive = false;
		entry.generation++;
	}
}