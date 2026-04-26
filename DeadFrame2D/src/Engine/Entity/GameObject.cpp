#include "Engine/ECS/Component/Transform.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectHierarchyChangeEvent.h"
#include "Engine/Entity/GameObject.h"
#include "Engine/SceneSystem/Scene.h"
#include "Engine/SceneSystem/SceneManager.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"
#include <stack>


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Utilities;


	GameObject::GameObject()
		: isDestroyed(false),
		isActive(true),
		hasActiveParent(true)
	{
		children.clear();

		componentBucket = std::make_shared<ComponentBucket>();

		transform = AddComponent<Transform>();
	}

	void GameObject::PropagateActiveStateToChildren()
	{
		for (const auto& child : children)
		{
			if (child == nullptr || child->isDestroyed)
				continue;

			bool oldState = child->IsActive();

			// Update child's parent state before re-evaluating
			child->hasActiveParent = IsActive();

			bool newState = child->IsActive();

			if (oldState != newState)
			{
				child->OnActiveStateChanged.Broadcast(child, newState);

				child->PropagateActiveStateToChildren();
			}
		}
	}

	Scene* GameObject::SafeGetActiveScene()
	{
		auto activeScene = SceneManager::GetActiveScene();

		if (!activeScene)
		{
			throw std::runtime_error("There is no active scene! Load a scene before instantiating a GameObject!");
		}

		return const_cast<Scene*>(activeScene);
	}

	void GameObject::ConstructGameObject()
	{
		// Meant for prefabs/blueprints
	}

	bool GameObject::IsChildOf(ObjectHandle<GameObject> potentialChild, bool recursive) const
	{
		if (potentialChild == nullptr)
			return false;

		if (parent == nullptr)
			return false;

		if (parent == potentialChild)
			return true;

		if (!recursive)
			return false;

		return parent->IsChildOf(potentialChild, true);
	}

	void GameObject::Destroy()
	{
		if (isDestroyed)
			return;

		if (parent != nullptr)
		{
			auto it = std::find(
				parent->children.begin(),
				parent->children.end(),
				thisGameObject);

			if (it != parent->children.end())
			{
				parent->children.erase(it);
			}
		}

		ObjectHandle<GameObject> current = parent;

		while (current != nullptr)
		{
			current->OnChildDestroyed.Broadcast(thisGameObject);

			current = current->parent;
		}

		for (auto& child : children)
		{
			if (child == nullptr)
				continue;

			child->Destroy();
		}

		EventDispatcher::SendEvent(std::make_shared<GameObjectDestroyedEvent>(thisGameObject));

		isDestroyed = true;
	}

	Task GameObject::Destroy(float delaySeconds)
	{
		if (delaySeconds > 0.0f)
		{
			co_await WaitSecondsUnscaled(delaySeconds);
		}

		Destroy();
	}

	ComponentHandle<Transform> GameObject::GetTransform() const
	{
		return transform;
	}

	bool GameObject::IsActive() const
	{
		return isActive && hasActiveParent;
	}

	void GameObject::SetActive(bool value)
	{
		auto oldState = IsActive();

		isActive = value;

		auto newState = IsActive();

		// Only notify if it changed
		if (oldState != newState)
		{
			OnActiveStateChanged.Broadcast(thisGameObject, newState);

			ObjectHandle<GameObject> current = parent;
			while (current != nullptr)
			{
				current->OnChildActiveStateChanged.Broadcast(thisGameObject, newState);

				current = current->parent;
			}
		}

		for (const auto& child : children) 
		{
			if (child == nullptr || child->isDestroyed)
				continue;

			child->hasActiveParent = newState;
			child->PropagateActiveStateToChildren();
		}
	}

	void GameObject::SetParent(ObjectHandle<GameObject> newParent)
	{
		if (parent == newParent)
			return;

		// Cache transform before reparenting
		auto transform = GetTransform()();

		const auto worldPos = transform->GetWorldPosition();
		const auto worldScale = transform->GetWorldScale();
		const auto worldRot = transform->GetWorldRotation();

		// Detach from old parent
		if (parent != nullptr)
		{
			auto& siblings = parent->children;

			siblings.erase(
				std::remove_if(
					siblings.begin(),
					siblings.end(),
					[&](const auto& obj)
					{
						return obj == thisGameObject;
					}),
				siblings.end());
		}

		auto oldParent = parent;

		// Attach to new parent
		parent = newParent;
		newParent->children.push_back(thisGameObject);

		// Restore world transform
		transform->SetWorldPosition(worldPos);
		transform->SetWorldScale(worldScale);
		transform->SetWorldRotation(worldRot);

		// Active state propagation
		hasActiveParent = newParent->IsActive();
		PropagateActiveStateToChildren();

		// Notify parents up the chain
		for (auto current = newParent; current != nullptr; current = current->parent)
		{
			current->OnChildGameObjectAdded.Broadcast(thisGameObject);
		}

		// Notify children of parent change
		std::stack<ObjectHandle<GameObject>> stack;
		for (auto& child : children)
		{
			stack.push(child);
		}

		while (!stack.empty())
		{
			ObjectHandle<GameObject> current = stack.top();
			
			stack.pop();

			current->OnParentGameObjectChanged.Broadcast(newParent);

			for (auto& c : current->children)
			{
				stack.push(c);
			}
		}

		// Event
		EventDispatcher::SendEvent(std::make_shared<GameObjectHierarchyChangeEvent>(thisGameObject, oldParent, newParent));
	}
}