#pragma once
#include "Core/Context/Systems/Coroutines/Task.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Core/GameObjectNotifier.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class Transform;

	class Scene;

	class ComponentBucket;


	class DF2D_API GameObject : public GameObjectNotifier
	{
		friend class Scene;


	private:
		bool isDestroyed;


		void PropagateActiveStateToChildren();


		static Scene* SafeGetActiveScene();


	protected:
		bool isActive;

		bool hasActiveParent;

		ComponentHandle<Transform> transform;

		std::shared_ptr<ComponentBucket> componentBucket;


		GameObject();


		virtual void ConstructGameObject();


	public:
		virtual ~GameObject() override = default;


		template<typename T, typename ...Args>
		static ObjectHandle<T> Instantiate(Args&&... args);


		template <typename T>
		ComponentHandle<T> GetComponent() const;

		template <typename T>
		ComponentHandle<T> GetComponentInChildren(bool recursive = false) const;

		template <typename T>
		std::vector<ComponentHandle<T>> GetComponentsInChildren(bool recursive = false) const;

		template<typename T>
		ComponentHandle<T> GetComponentInParent(bool recursive = false, bool includeSelf = false) const;

		template<typename T>
		std::vector<ComponentHandle<T>> GetComponentsInParent(bool recursive = false) const;

		template<typename T, typename... TArgs>
		ComponentHandle<T> AddComponent(TArgs&& ...args);

		template<typename T>
		void RemoveComponent(const ComponentHandle<T>& handle);

		bool IsChildOf(ObjectHandle<GameObject> potentialChild, bool recursive = false) const;

		void Destroy();

		DeadFrame2D::Core::Task Destroy(float delaySeconds);


		ComponentHandle<Transform> GetTransform() const;

		bool IsActive() const;

		void SetActive(bool value);

		void SetParent(ObjectHandle<GameObject> child);
	};
}


#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/System/Scene/Scene.h"


namespace DeadFrame2D::Engine
{
	template<typename T, typename ...Args>
	inline ObjectHandle<T> GameObject::Instantiate(Args && ...args)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

		return SafeGetActiveScene()->template Instantiate<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	inline ComponentHandle<T> GameObject::GetComponent() const
	{
		return componentBucket->GetComponent<T>();
	}

	template<typename T>
	inline ComponentHandle<T> GameObject::GetComponentInChildren(bool recursive) const
	{
		for (const auto& child : children)
		{
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

		for (const auto& child : children)
		{
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
	inline ComponentHandle<T> GameObject::GetComponentInParent(bool recursive, bool includeSelf) const
	{
		ObjectHandle<GameObject> current = includeSelf ? thisGameObject : parent;

		while (current != nullptr)
		{
			if (auto comp = current->GetComponent<T>())
				return comp;

			if (!recursive)
				break;

			current = current->parent;
		}

		return ComponentHandle<T>();
	}

	template<typename T>
	inline std::vector<ComponentHandle<T>> GameObject::GetComponentsInParent(bool recursive) const
	{
		std::vector<ComponentHandle<T>> results;

		auto current = parent;

		while (current != nullptr)
		{
			if (auto comp = current->GetComponent<T>())
				results.push_back(comp);

			if (!recursive)
				break;

			current = current->parent;
		}

		return results;
	}

	template<typename T, typename ...TArgs>
	inline ComponentHandle<T> GameObject::AddComponent(TArgs && ...args)
	{
		static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

		auto newComponent = componentBucket->AddComponent<T>(thisGameObject, std::forward<TArgs>(args)...);

		OnNewComponentAdded.Broadcast(ComponentHandle<GameComponent>::From(newComponent));

		return newComponent;
	}

	template<typename T>
	inline void GameObject::RemoveComponent(const ComponentHandle<T>& handle)
	{
		OnComponentRemoved(handle);

		componentBucket->RemoveComponent<T>(handle);
	}
}