#pragma once
#include "ComponentBucket.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "IObject.h"
#include <memory>
#include <vector>


class Transform;


class GameObject : public IObject
{
private:
	bool isDestroyed;


protected:
	GameObject();


	Transform* transform;

	ComponentBucket componentBucket;


public:
	virtual ~GameObject() = default;

	
	virtual void Update(float dt) override;
	
	virtual void Draw() override;


	template <typename T>
	T* GetComponent() const;
	
	template<typename T, typename... TArgs>
	T* AddComponent(TArgs&& ...args);


	template<typename T, typename ...Args>
	static std::weak_ptr<T> Instantiate(Args && ...args);
	

	void Destroy();
};


template<typename T>
inline T* GameObject::GetComponent() const
{
	return componentBucket.GetComponent<T>();
}

template<typename T, typename... TArgs>
inline T* GameObject::AddComponent(TArgs&& ...args)
{
	return componentBucket.AddComponent<T>(this, std::forward<TArgs>(args)...);
}

template<typename T, typename ...Args>
inline std::weak_ptr<T> GameObject::Instantiate(Args && ...args)
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

	auto obj = std::make_shared<T>(std::forward<Args>(args)...);

	EventDispatcher::SendEvent(std::make_shared<GameObjectCreatedEvent>(obj));

	return obj;
}