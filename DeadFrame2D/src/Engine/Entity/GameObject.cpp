#include "Engine/Components/Transform.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/ChildGameObjectAddedEvent.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Utilities;


	GameObject::GameObject()
		: isDestroyed(false),
		isInitialized(false),
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
			auto childPtr = child.lock();

			if (childPtr == nullptr || childPtr->isDestroyed)
				continue;

			bool oldState = childPtr->IsActive();

			// Update child's parent state before re-evaluating
			childPtr->hasActiveParent = IsActive();

			bool newState = childPtr->IsActive();

			if (oldState != newState)
			{
				childPtr->OnActiveStateChanged(childPtr.get(), newState);

				childPtr->PropagateActiveStateToChildren();
			}
		}
	}

	void GameObject::ConstructGameObject()
	{
		// Meant for prefabs/blueprints
	}

	void GameObject::Init()
	{
		if (isInitialized)
			return;

		componentBucket->ForEach([](GameComponent& comp) 
		{
			comp.Init();
		});

		isInitialized = true;
	}

	void GameObject::Start()
	{
		componentBucket->ForEach([](GameComponent& comp)
		{
			comp.Start();
		});
	}

	void GameObject::Update(float deltaTime)
	{
		componentBucket->ForEach([deltaTime](GameComponent& comp)
		{
			if (!comp.IsActive())
				return;

			comp.Update(deltaTime);
		});

		for (const auto& child : children)
		{
			auto childPtr = child.lock();

			if (childPtr == nullptr)
				continue;

			childPtr->Update(deltaTime);
		}
	}

	void GameObject::LateUpdate(float deltaTime)
	{
		componentBucket->ForEach([deltaTime](GameComponent& comp)
		{
			if (!comp.IsActive())
				return;

			comp.LateUpdate(deltaTime);
		});

		for (const auto& child : children)
		{
			auto childPtr = child.lock();

			if (childPtr == nullptr)
				continue;

			childPtr->LateUpdate(deltaTime);
		}
	}

	void GameObject::Draw()
	{
		componentBucket->ForEach([](GameComponent& comp)
		{
			if (!comp.IsActive())
				return;

			comp.Draw();
		});

		for (const auto& child : children)
		{
			auto childPtr = child.lock();
		
			if (childPtr == nullptr || !childPtr->IsActive())
				continue;

			childPtr->Draw();
		}
	}

	void GameObject::AddChildGameObject(std::weak_ptr<GameObject> child)
	{
		auto childPtr = child.lock();
		if (!childPtr)
			return;

		// Step 1: Cache world transform before parenting
		auto transform = childPtr->GetComponent<Transform>();
	
		auto worldPos = transform->GetWorldPosition();
		auto worldScale = transform->GetWorldScale();
		auto worldRot = transform->GetWorldRotation();

		// Step 2: Remove from previous parent
		auto parentPtr = childPtr->parent.lock();
		if (parentPtr != nullptr)
		{
			auto& siblings = parentPtr->children;

			siblings.erase(
				std::remove_if(siblings.begin(), siblings.end(),
					[&](const std::weak_ptr<GameObject>& weakSibling)
					{
						auto sibling = weakSibling.lock();
						return sibling && sibling.get() == childPtr.get();
					}),
				siblings.end());
		}

		// Step 3: Reparent
		childPtr->parent = thisWeak;
		children.push_back(child);

		// Step 4: Convert world transform back to local under new parent
		transform->SetWorldPosition(worldPos);
		transform->SetWorldScale(worldScale);
		transform->SetWorldRotation(worldRot);

		// Propagate active state
		childPtr->hasActiveParent = IsActive();
		PropagateActiveStateToChildren();

		EventDispatcher::SendEvent(std::make_shared<ChildGameObjectAddedEvent>(child));
	}

	bool GameObject::IsChildOf(std::weak_ptr<GameObject> potentialChild, bool recursive) const
	{
		auto potentialChildPtr = potentialChild.lock();
		if (potentialChildPtr == nullptr)
			return false;

		auto currentParentPtr = parent.lock();
		if (currentParentPtr == nullptr)
			return false;

		if (currentParentPtr == potentialChildPtr)
			return true;

		if (!recursive)
			return false;

		return currentParentPtr->IsChildOf(potentialChildPtr, true);
	}

	void GameObject::Destroy()
	{
		if (isDestroyed)
			return;

		isDestroyed = true;

		EventDispatcher::SendEvent(std::make_shared<GameObjectDestroyedEvent>(thisWeak));

		auto parentPtr = parent.lock();
		if (parentPtr != nullptr)
		{
			auto it = std::find_if(
				parentPtr->children.begin(),
				parentPtr->children.end(),
				[this](const std::weak_ptr<GameObject>& child)
				{
					return child.lock() == thisWeak.lock();
				});

			if (it != parentPtr->children.end())
			{
				parentPtr->children.erase(it);
			}
		}

		for (auto& child : children)
		{
			auto childPtr = child.lock();

			if (childPtr != nullptr)
				childPtr->Destroy();
		}
	}

	Task GameObject::Destroy(float delaySeconds)
	{
		if (delaySeconds > 0.0f)
		{
			co_await WaitSeconds(delaySeconds);
		}

		Destroy();
	}

	std::weak_ptr<GameObject> GameObject::GetThisWeak() const
	{
		return thisWeak;
	}

	std::weak_ptr<GameObject> GameObject::GetParent() const
	{
		return parent;
	}

	ComponentHandle<Transform> GameObject::GetTransform() const
	{
		return transform;
	}

	std::vector<std::weak_ptr<GameObject>> GameObject::GetChildren() const
	{
		return children;
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
			OnActiveStateChanged(this, newState);
		}

		for (const auto& child : children) 
		{
			auto childPtr = child.lock();

			if (childPtr == nullptr || childPtr->isDestroyed)
				continue;

			childPtr->hasActiveParent = newState;
			childPtr->PropagateActiveStateToChildren();
		}
	}
}