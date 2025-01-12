#include <EventSystem/EventDispatcher.h>
#include <Components/Transform.h>
#include <GameObject.h>
#include <EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h>
#include <EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h>


GameObject::GameObject()
{
	transform = &AddComponent<Transform>();

	EventDispatcher::SendEvent(std::make_shared<GameObjectCreatedEvent>(this));
}

GameObject::~GameObject()
{
	Clean();
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
	EventDispatcher::SendEvent(std::make_shared<GameObjectDestroyedEvent>(this));
}

void GameObject::Clean()
{
}