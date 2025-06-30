#include "Components/Transform.h"
#include "EventSystem/Events/GameObjectEvents/ChildGameObjectAddedEvent.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "GameObject.h"
#include "Tools/Helpers/Coroutines/CoroutineHelpers.h"


GameObject::GameObject()
	: isDestroyed(false),
	isInitialized(false),
	isActive(true),
	hasActiveParent(true)
{
	children.clear();

	transform = AddComponent<Transform>();
}

void GameObject::PropagateActiveStateToChildren()
{
	bool newParentState = IsActive(); // This object’s own active state including its parents

	OnActiveStateChanged(this, newParentState);

	for (const auto& child : children)
	{
		auto childPtr = child.lock();

		if (childPtr == nullptr || childPtr->isDestroyed)
			continue;

		childPtr->hasActiveParent = newParentState;
		childPtr->PropagateActiveStateToChildren();
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

	for (const auto& component : componentBucket.GetComponents())
	{
		component->Init();
	}

	isInitialized = true;
}

void GameObject::Start()
{
	for (const auto& component : componentBucket.GetComponents())
	{
		component->Start();
	}
}

void GameObject::Update(float deltaTime)
{
	for (const auto& component : componentBucket.GetComponents())
	{
		if (!component->IsActive())
			continue;

		component->Update(deltaTime);
	}

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
	for (const auto& component : componentBucket.GetComponents())
	{
		if (!component->IsActive())
			continue;

		// TODO: implement late update in GameComponents
		//component->LateUpdate(deltaTime);
	}

	for (const auto& child : children)
	{
		auto childPtr = child.lock();

		if (childPtr == nullptr)
			continue;

		child.lock()->LateUpdate(deltaTime);
	}
}

void GameObject::Draw()
{
	for (auto& component : componentBucket.GetComponents())
	{
		if (!component->IsActive())
			continue;

		component->Draw();
	}

	for (const auto& child : children)
	{
		auto childPtr = child.lock();
		
		if (childPtr == nullptr || !childPtr->IsActive())
			continue;

		child.lock()->Draw();
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
	childPtr->hasActiveParent = this->IsActive();
	PropagateActiveStateToChildren();

	// Clear 'OnActiveStateChanged' callback
	OnActiveStateChanged.Clear();

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
		co_await Tools::Helpers::Coroutines::WaitSeconds(delaySeconds);
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

Transform* GameObject::GetTransform() const
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
	isActive = value;

	// this includes our own parent's state
	auto newParentState = IsActive(); 

	OnActiveStateChanged(this, newParentState);

	for (const auto& child : children) 
	{
		auto childPtr = child.lock();

		if (childPtr == nullptr || childPtr->isDestroyed)
			continue;

		childPtr->hasActiveParent = newParentState;
		childPtr->PropagateActiveStateToChildren();
	}
}