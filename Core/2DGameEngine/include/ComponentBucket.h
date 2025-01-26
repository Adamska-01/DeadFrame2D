#pragma once
#include "Components/GameComponent.h"
#include <memory>
#include <vector>


class GameObject;


class ComponentBucket
{
private:
	std::vector<std::unique_ptr<GameComponent>> components;


public:
	ComponentBucket() = default;

	~ComponentBucket();
	
	
	template <typename T>
	T* GetComponent() const;

	template<typename T, typename... TArgs>
	T* AddComponent(GameObject* owner, TArgs&& ...args);


	const std::vector<std::unique_ptr<GameComponent>>& GetComponents();
};


inline ComponentBucket::~ComponentBucket()
{
	components.clear();
}

template<typename T>
T* ComponentBucket::GetComponent() const
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
T* ComponentBucket::AddComponent(GameObject* owner, TArgs&& ...args)
{
	auto existingComponent = GetComponent<T>();

	if (existingComponent != nullptr)
		return existingComponent;

	T* component = new T(std::forward<TArgs>(args)...);

	component->OwningObject = owner;

	std::unique_ptr<GameComponent> uniquePtr(component);

	static_assert(std::is_base_of<GameComponent, T>::value, "child must inherit from GameComponent");
	static_assert(std::is_same<std::unique_ptr<GameComponent>, decltype(uniquePtr)>::value, "uniquePtr must be of the correct type");

	components.emplace_back(std::move(uniquePtr));

	component->Init();

	return component;
}

inline const std::vector<std::unique_ptr<GameComponent>>& ComponentBucket::GetComponents()
{
	return components;
}