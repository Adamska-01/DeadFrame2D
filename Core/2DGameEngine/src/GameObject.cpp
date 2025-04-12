#include "Components/Transform.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "GameObject.h"


GameObject::GameObject()
	: isDestroyed(false),
	isInitialized(false)
{
	transform = AddComponent<Transform>();
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
		component->Update(dt);
	}
}

void GameObject::Draw()
{
	transform->Draw();

	for (auto& component : componentBucket.GetComponents())
	{
		component->Draw();
	}
}

void GameObject::Destroy()
{
	if (isDestroyed)
		return;

	isDestroyed = true;

	EventDispatcher::SendEvent(std::make_shared<GameObjectDestroyedEvent>(this));
}