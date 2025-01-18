#include "Components/Transform.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "GameObject.h"


GameObject::GameObject()
	: isDestroyed(false)
{
	transform = &AddComponent<Transform>();
}

void GameObject::Update(float dt)
{
	transform->Update(dt);

	for (auto& component : componentBucket.components)
	{
		component->Update(dt);
	}
}

void GameObject::Draw()
{
	transform->Draw();

	for (auto& component : componentBucket.components)
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