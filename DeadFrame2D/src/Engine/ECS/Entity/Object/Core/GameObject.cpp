#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/Entity/Object/Core/GameObjectConstructionContext.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/GameObject/GameObjectDestroyedEvent.h"
#include "Engine/Events/GameObject/GameObjectHierarchyChangeEvent.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"
#include <stack>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Utilities;


	GameObject::GameObject()
		: isDestroyed(false),
		isActive(true),
		hasActiveParent(true)
	{
		children.clear();

		componentBucket = std::make_shared<ComponentBucket>();

		const auto* ctx = GameObjectConstructionContext::Current();

		if (ctx != nullptr)
		{
			SetThisHandle(ObjectHandle<GameObject>(ctx->handle));
			SetContexts(ctx->coreCtx, ctx->serviceCtx);
		}

		transform = AddComponent<Transform>();
	}

	void GameObject::PropagateActiveStateToChildren()
	{
		for (const auto& child : children)
		{
			if (child == nullptr || child->isDestroyed)
				continue;

			auto oldState = child->IsActive();

			// Update child's parent state before re-evaluating
			child->hasActiveParent = IsActive();

			auto newState = child->IsActive();

			if (oldState != newState)
			{
				child->OnActiveStateChanged.Broadcast(child, newState);

				child->PropagateActiveStateToChildren();
			}
		}
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

		if (serviceCtx.eventDispatcher)
		{
			serviceCtx.eventDispatcher->SendEvent(std::make_shared<GameObjectDestroyedEvent>(thisGameObject));
		}

		isDestroyed = true;
	}

	Task GameObject::Destroy(float delaySeconds)
	{
		if (delaySeconds > 0.0f)
		{
			co_await CoroutineHelpers::WaitSecondsUnscaled(delaySeconds);
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

		if (serviceCtx.eventDispatcher)
		{
			serviceCtx.eventDispatcher->SendEvent(std::make_shared<GameObjectHierarchyChangeEvent>(thisGameObject, oldParent, newParent));
		}
	}
}