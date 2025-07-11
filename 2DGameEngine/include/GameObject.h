#pragma once
#include "ComponentBucket.h"
#include "Coroutines/Task.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "GameObjectNotifier.h"
#include "IObject.h"
#include <memory>


class Transform;


class GameObject : public IObject, public GameObjectNotifier
{
private:
	bool isInitialized;

	bool isDestroyed;


	void PropagateActiveStateToChildren();


protected:
	bool isActive;

	bool hasActiveParent;

	std::weak_ptr<GameObject> thisWeak;

	std::weak_ptr<GameObject> parent;

	std::vector<std::weak_ptr<GameObject>> children;

	Transform* transform;

	ComponentBucket componentBucket;


	GameObject();


	virtual void ConstructGameObject();


public:
	virtual ~GameObject() override = default;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;
	
	virtual void LateUpdate(float deltaTime) override;
	
	virtual void Draw() override;


	template<typename T, typename ...Args>
	static std::weak_ptr<T> Instantiate(Args && ...args);


	template <typename T>
	T* GetComponent() const;

	template <typename T>
	T* GetComponentInChildren(bool recursive = false) const;

	template <typename T>
	std::vector<T*> GetComponentsInChildren(bool recursive = false) const;

	template<typename T>
	inline T* GetComponentInParent(bool recursive = false) const;

	template<typename T>
	inline std::vector<T*> GetComponentsInParent(bool recursive = false) const;
	
	template<typename T, typename... TArgs>
	T* AddComponent(TArgs&& ...args);

	void AddChildGameObject(std::weak_ptr<GameObject> child);

	bool IsChildOf(std::weak_ptr<GameObject> potentialChild, bool recursive = false) const;

	void Destroy();

	Task Destroy(float delaySeconds);


	std::weak_ptr<GameObject> GetThisWeak() const;

	std::weak_ptr<GameObject> GetParent() const;

	Transform* GetTransform() const;

	std::vector<std::weak_ptr<GameObject>> GetChildren() const;
	
	bool IsActive() const;

	void SetActive(bool value);
};


template<typename T, typename ...Args>
inline std::weak_ptr<T> GameObject::Instantiate(Args && ...args)
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

	auto obj = std::shared_ptr<T>(new T(std::forward<Args>(args)...));

	obj->thisWeak = obj;

	// This is necessary due to smart pointer/C++ limitations
	for (auto& component : obj->componentBucket.GetComponents())
	{
		obj->componentBucket.LinkComponentToOwner(obj, component.get());
	}

	EventDispatcher::SendEvent(std::make_shared<GameObjectCreatedEvent>(obj));

	obj->ConstructGameObject();

	return obj;
}

template<typename T>
inline T* GameObject::GetComponent() const
{
	return componentBucket.GetComponent<T>();
}

template<typename T>
inline T* GameObject::GetComponentInChildren(bool recursive) const
{
	for (const auto& weakChild : children)
	{
		auto child = weakChild.lock();

		if (child == nullptr || child->isDestroyed)
			continue;

		if (T* comp = child->GetComponent<T>())
			return comp;

		if (recursive)
		{
			if (T* childComp = child->GetComponentInChildren<T>(true))
				return childComp;
		}
	}

	return nullptr;
}

template<typename T>
inline std::vector<T*> GameObject::GetComponentsInChildren(bool recursive) const
{
	std::vector<T*> results;

	for (const auto& weakChild : children)
	{
		auto child = weakChild.lock();

		if (child == nullptr || child->isDestroyed)
			continue;

		if (T* comp = child->GetComponent<T>())
			results.push_back(comp);

		if (recursive)
		{
			auto childResults = child->GetComponentsInChildren<T>(true);

			results.insert(results.end(), childResults.begin(), childResults.end());
		}
	}

	return results;
}

template<typename T>
inline T* GameObject::GetComponentInParent(bool recursive) const
{
	auto current = parent.lock();

	while (current != nullptr)
	{
		if (T* comp = current->GetComponent<T>())
			return comp;

		if (!recursive)
			break;

		current = current->parent.lock();
	}

	return nullptr;
}

template<typename T>
inline std::vector<T*> GameObject::GetComponentsInParent(bool recursive) const
{
	std::vector<T*> results;
	auto current = parent.lock();

	while (current != nullptr)
	{
		if (T* comp = current->GetComponent<T>())
			results.push_back(comp);

		if (!recursive)
			break;

		current = current->parent.lock();
	}

	return results;
}

template<typename T, typename... TArgs>
inline T* GameObject::AddComponent(TArgs&& ...args)
{
	static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

	auto newComponent = componentBucket.AddComponent<T>(thisWeak, isInitialized, std::forward<TArgs>(args)...);

	OnNewComponentAdded(newComponent);

	return newComponent;
}