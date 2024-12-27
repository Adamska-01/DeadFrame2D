#pragma once
#include "ComponentBucket.h"
#include "IObject.h"
#include <memory>
#include <vector>


class Transform;


class GameObject : public IObject
{
public:
	GameObject();

	~GameObject();

	
	virtual void Update(float dt) override;
	
	virtual void Draw() override;


	template <typename T>
	bool ContainsComponent() const;

	template <typename T>
	T& GetComponent() const;
	
	template<typename T, typename... TArgs>
	T& AddComponent(TArgs&& ...args);
	

protected:
	Transform* transform;

	ComponentBucket componentBucket;

	
	virtual void Clean() override;
};


template<typename T>
bool GameObject::ContainsComponent() const
{
	return componentBucket.ContainsComponent<T>();
}

template<typename T>
T& GameObject::GetComponent() const
{
	return componentBucket.GetComponent<T>();
}

template<typename T, typename... TArgs>
T& GameObject::AddComponent(TArgs&& ...args)
{
	return componentBucket.AddComponent<T>(this, std::forward<TArgs>(args)...);
}