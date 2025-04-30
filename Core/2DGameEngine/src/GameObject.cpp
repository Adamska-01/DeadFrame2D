#include "Components/Transform.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "GameObject.h"


GameObject::GameObject()
	: isDestroyed(false),
	isInitialized(false),
	isActive(true),
	hasActiveParent(true),
	parent(nullptr)
{
	children.clear();

	transform = AddComponent<Transform>();
}

void GameObject::PropagateActiveStateToChildren()
{
	bool newParentState = IsActive(); // This object’s own active state including its parents

	for (const auto& child : children)
	{
		auto childPtr = child.lock();

		if (childPtr == nullptr || childPtr->isDestroyed)
			continue;

		childPtr->hasActiveParent = newParentState;
		childPtr->PropagateActiveStateToChildren();
	}	
}

void GameObject::Init()
{
	if (isInitialized)
		return;

	for (auto& component : componentBucket.GetComponents())
	{
		component->Init();
	}

	isInitialized = true;
}

void GameObject::Update(float dt)
{
	for (auto& component : componentBucket.GetComponents())
	{
		if (!component->IsActive())
			continue;

		component->Update(dt);
	}
}

void GameObject::LateUpdate(float deltaTime)
{
}

void GameObject::Draw()
{
	for (auto& component : componentBucket.GetComponents())
	{
		if (!component->IsActive())
			continue;

		component->Draw();
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
	if (childPtr->parent)
	{
		auto& siblings = childPtr->parent->children;

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
	childPtr->parent = this;
	children.push_back(child);

	// Step 4: Convert world transform back to local under new parent
	transform->SetWorldPosition(worldPos);
	transform->SetWorldScale(worldScale);
	transform->SetWorldRotation(worldRot);

	// Propagate active state
	childPtr->hasActiveParent = this->IsActive();
	PropagateActiveStateToChildren();
}

bool GameObject::IsChildOf(const GameObject* potentialChild, bool recursive) const
{
	if (potentialChild == nullptr)
		return false;

	auto currentParent = parent;
	if (currentParent == nullptr)
		return false;

	if (currentParent == potentialChild)
		return true;

	if (!recursive)
		return false;

	return currentParent->IsChildOf(potentialChild, true);
}

void GameObject::Destroy()
{
	if (isDestroyed)
		return;

	isDestroyed = true;

	EventDispatcher::SendEvent(std::make_shared<GameObjectDestroyedEvent>(this));

	for (auto& child : children)
	{
		auto childPtr = child.lock();

		if (childPtr != nullptr)
			childPtr->Destroy();
	}
}

GameObject* GameObject::GetParent() const
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