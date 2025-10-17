#pragma once
#include "Core/Coroutines/Task.h"
#include "DF2D_API.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "Engine/Entity/Abstractions/IObject.h"
#include "Engine/Entity/ComponentBucket.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Engine/Entity/GameObjectNotifier.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class Transform;


	class DF2D_API GameObject : public IObject, public GameObjectNotifier
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

		ComponentHandle<Transform> transform;

		std::shared_ptr<ComponentBucket> componentBucket;


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
		ComponentHandle<T> GetComponent() const;

		template <typename T>
		ComponentHandle<T> GetComponentInChildren(bool recursive = false) const;

		template <typename T>
		std::vector<ComponentHandle<T>> GetComponentsInChildren(bool recursive = false) const;

		template<typename T>
		ComponentHandle<T> GetComponentInParent(bool recursive = false) const;

		template<typename T>
		std::vector<ComponentHandle<T>> GetComponentsInParent(bool recursive = false) const;

		template<typename T, typename... TArgs>
		ComponentHandle<T> AddComponent(TArgs&& ...args);

		template<typename T>
		void RemoveComponent(const ComponentHandle<T>& handle);

		void AddChildGameObject(std::weak_ptr<GameObject> child);

		bool IsChildOf(std::weak_ptr<GameObject> potentialChild, bool recursive = false) const;

		void Destroy();

		DeadFrame2D::Core::Task Destroy(float delaySeconds);


		std::weak_ptr<GameObject> GetThisWeak() const;

		std::weak_ptr<GameObject> GetParent() const;

		ComponentHandle<Transform> GetTransform() const;

		std::vector<std::weak_ptr<GameObject>> GetChildren() const;
		bool IsActive() const;

		void SetActive(bool value);
	};
}


namespace DeadFrame2D::Engine
{
	template<typename T, typename ...Args>
	inline std::weak_ptr<T> GameObject::Instantiate(Args && ...args)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

		auto obj = std::shared_ptr<T>(new T(std::forward<Args>(args)...));

		obj->thisWeak = obj;

		// This is necessary due to smart pointer/C++ limitations
		obj->componentBucket->ForEach([&obj](GameComponent& comp) 
		{
			obj->componentBucket->LinkComponentToOwner(obj, &comp);
		});

		EventDispatcher::SendEvent(std::make_shared<GameObjectCreatedEvent>(obj));

		obj->ConstructGameObject();

		return obj;
	}

	template<typename T>
	inline ComponentHandle<T> GameObject::GetComponent() const
	{
		return componentBucket->GetComponent<T>();
	}

	template<typename T>
	inline ComponentHandle<T> GameObject::GetComponentInChildren(bool recursive) const
	{
		for (const auto& weakChild : children)
		{
			auto child = weakChild.lock();

			if (child == nullptr || child->isDestroyed)
				continue;

			if (auto comp = child->GetComponent<T>())
				return comp;

			if (recursive)
			{
				if (auto childComp = child->GetComponentInChildren<T>(true))
					return childComp;
			}
		}

		return ComponentHandle<T>();
	}

	template<typename T>
	inline std::vector<ComponentHandle<T>> GameObject::GetComponentsInChildren(bool recursive) const
	{
		std::vector<ComponentHandle<T>> results;

		for (const auto& weakChild : children)
		{
			auto child = weakChild.lock();

			if (child == nullptr || child->isDestroyed)
				continue;

			if (auto comp = child->GetComponent<T>())
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
	inline ComponentHandle<T> GameObject::GetComponentInParent(bool recursive) const
	{
		auto current = parent.lock();

		while (current != nullptr)
		{
			if (auto comp = current->GetComponent<T>())
				return comp;

			if (!recursive)
				break;

			current = current->parent.lock();
		}

		return ComponentHandle<T>();
	}

	template<typename T>
	inline std::vector<ComponentHandle<T>> GameObject::GetComponentsInParent(bool recursive) const
	{
		std::vector<ComponentHandle<T>> results;

		auto current = parent.lock();

		while (current != nullptr)
		{
			if (auto comp = current->GetComponent<T>())
				results.push_back(comp);

			if (!recursive)
				break;

			current = current->parent.lock();
		}

		return results;
	}

	template<typename T, typename ...TArgs>
	inline ComponentHandle<T> GameObject::AddComponent(TArgs && ...args)
	{
		static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

		auto newComponent = componentBucket->AddComponent<T>(thisWeak, isInitialized, std::forward<TArgs>(args)...);

		OnNewComponentAdded(ComponentHandle<GameComponent>::From(newComponent));

		return newComponent;
	}

	template<typename T>
	inline void GameObject::RemoveComponent(const ComponentHandle<T>& handle)
	{
		OnComponentRemoved(handle);

		componentBucket->RemoveComponent<T>(handle);
	}
}