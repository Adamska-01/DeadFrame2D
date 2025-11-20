#include "Engine/Components/Transform.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/ChildGameObjectAddedEvent.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Engine/Entity/GameObject.h"
#include "Engine/SceneSystem/Scene.h"
#include "Engine/SceneSystem/SceneManager.h"
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
			if (child == nullptr || child->isDestroyed)
				continue;

			bool oldState = child->IsActive();

			// Update child's parent state before re-evaluating
			child->hasActiveParent = IsActive();

			bool newState = child->IsActive();

			if (oldState != newState)
			{
				child->OnActiveStateChanged(child, newState);

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
			if (child == nullptr || !child->IsActive())
				continue;

			child->Update(deltaTime);
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
			if (child == nullptr || !child->IsActive())
				continue;

			child->LateUpdate(deltaTime);
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
			if (child == nullptr || !child->IsActive())
				continue;

			child->Draw();
		}
	}

	void GameObject::AddChildGameObject(ObjectHandle<GameObject> child)
	{
		if (child == nullptr)
			return;

		// Step 1: Cache world transform before parenting
		auto transform = child->GetTransform();
	
		auto worldPos = transform->GetWorldPosition();
		auto worldScale = transform->GetWorldScale();
		auto worldRot = transform->GetWorldRotation();

		// Step 2: Remove from previous parent
		auto parentPtr = child->parent;
		if (parentPtr != nullptr)
		{
			auto& siblings = parentPtr->children;

			siblings.erase(
				std::remove_if(siblings.begin(), siblings.end(),
					[&](const ObjectHandle<GameObject>& sibling)
					{
						return sibling != nullptr && sibling == child;
					}),
				siblings.end());
		}

		// Step 3: Reparent
		child->parent = thisGameObject;
		children.push_back(child);

		// Step 4: Convert world transform back to local under new parent
		transform->SetWorldPosition(worldPos);
		transform->SetWorldScale(worldScale);
		transform->SetWorldRotation(worldRot);

		// Propagate active state
		child->hasActiveParent = IsActive();
		PropagateActiveStateToChildren();

		EventDispatcher::SendEvent(std::make_shared<ChildGameObjectAddedEvent>(child));
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

		isDestroyed = true;

		EventDispatcher::SendEvent(std::make_shared<GameObjectDestroyedEvent>(thisGameObject));

		if (parent != nullptr)
		{
			auto it = std::find_if(
				parent->children.begin(),
				parent->children.end(),
				[this](const ObjectHandle<GameObject>& child)
				{
					return child == thisGameObject;
				});

			if (it != parent->children.end())
			{
				parent->children.erase(it);
			}
		}

		for (auto& child : children)
		{
			if (child == nullptr)
				continue;

			child->Destroy();
		}
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
			OnActiveStateChanged(thisGameObject, newState);
		}

		for (const auto& child : children) 
		{
			if (child == nullptr || child->isDestroyed)
				continue;

			child->hasActiveParent = newState;
			child->PropagateActiveStateToChildren();
		}
	}
}