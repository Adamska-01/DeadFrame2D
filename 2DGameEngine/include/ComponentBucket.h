#pragma once
#include "Components/GameComponent.h"
#include "GameObject.h"
#include <memory>
#include <vector>


class ComponentBucket
{
private:
	std::vector<std::unique_ptr<GameComponent>> components;


public:
	ComponentBucket();

	~ComponentBucket();


	void LinkComponentToOwner(std::weak_ptr<GameObject> owner, GameComponent* toInitialize);
	

	template <typename T>
	T* GetComponent() const;

	template<typename T, typename... TArgs>
	T* AddComponent(std::weak_ptr<GameObject> owner, bool canInitialize, TArgs&& ...args);


	const std::vector<std::unique_ptr<GameComponent>>& GetComponents();
};


inline ComponentBucket::ComponentBucket()
{
	components.clear();
}

inline ComponentBucket::~ComponentBucket()
{
	components.clear();
}

inline void ComponentBucket::LinkComponentToOwner(std::weak_ptr<GameObject> owner, GameComponent* toInitialize)
{
	toInitialize->OwningObject = owner;

	toInitialize->RegisterAllHandlers(owner);
}

template<typename T>
inline T* ComponentBucket::GetComponent() const
{
	for (const auto& component : components)
	{
		T* castedComponent = dynamic_cast<T*>(component.get());
		
		if (castedComponent != nullptr) 
			return castedComponent;
	}

	// No matching component found
	return nullptr;
}

template<typename T, typename ...TArgs>
inline T* ComponentBucket::AddComponent(std::weak_ptr<GameObject> owner, bool canInitialize, TArgs&& ...args)
{
	auto existingComponent = GetComponent<T>();

	if (existingComponent != nullptr)
		return existingComponent;

	T* component = new T(std::forward<TArgs>(args)...);

	// This is necessary due to smart pointer/C++ limitations
	if (owner.lock() != nullptr)
	{
		LinkComponentToOwner(owner, component);
	}

	std::unique_ptr<GameComponent> uniquePtr(component);

	static_assert(std::is_base_of<GameComponent, T>::value, "child must inherit from GameComponent");
	static_assert(std::is_same<std::unique_ptr<GameComponent>, decltype(uniquePtr)>::value, "uniquePtr must be of the correct type");

	components.emplace_back(std::move(uniquePtr));

	if (canInitialize)
	{
		LinkComponentToOwner(owner, component);

		component->Init();
		component->Start();
	}

	return component;
}

inline const std::vector<std::unique_ptr<GameComponent>>& ComponentBucket::GetComponents()
{
	return components;
}