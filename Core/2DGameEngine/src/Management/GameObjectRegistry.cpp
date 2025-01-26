#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Management/GameObjectRegistry.h"
#include "EventSystem/EventDispatcher.h"
#include "Tools/Helpers/EventHelpers.h"


GameObjectRegistry::GameObjectRegistry()
{
	gameObjects.clear();
	colliders.clear();

	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &GameObjectRegistry::GameObjectCreatedHandler));
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &GameObjectRegistry::GameObjectDestroyedHandler));
}

GameObjectRegistry::~GameObjectRegistry()
{
	gameObjects.clear();
	colliders.clear();

	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &GameObjectRegistry::GameObjectCreatedHandler));
	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &GameObjectRegistry::GameObjectDestroyedHandler));
}

void GameObjectRegistry::Update(float deltaTime)
{
	for (const auto& obj : gameObjects)
	{
		obj->Update(deltaTime);
	}
}

void GameObjectRegistry::Draw()
{
	for (const auto& obj : gameObjects)
	{
		obj->Draw();
	}
}

void GameObjectRegistry::GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectCreatedEvent>(dispatchableEvent);

	if (gameObjEvent == nullptr || gameObjEvent->gameObjectCreated == nullptr)
		return;

	auto& target = gameObjEvent->gameObjectCreated;

	gameObjects.push_back(std::move(target));

	auto collider = gameObjects.back()->GetComponent<Collider2D>();

	if (collider == nullptr)
		return;

	colliders.push_back(collider);
}

void GameObjectRegistry::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

	if (gameObjEvent == nullptr || gameObjEvent->gameObjectDestroyed == nullptr)
		return;

	auto target = gameObjEvent->gameObjectDestroyed;
	auto targetCollider = target->GetComponent<Collider2D>();

	// Find and remove the collider reference associated with the target
	auto colliderIt = std::remove_if(
		colliders.begin(),
		colliders.end(),
		[targetCollider](Collider2D* collider)
		{
			return targetCollider == collider;
		});

	if (colliderIt != colliders.end())
	{
		colliders.erase(colliderIt, colliders.end());
	}

	// Now remove the target from gameObjects
	auto objIt = std::remove_if(
		gameObjects.begin(),
		gameObjects.end(),
		[target](std::shared_ptr<GameObject>& gameObject)
		{
			return gameObject.get() == target;
		});

	if (objIt != gameObjects.end())
	{
		gameObjects.erase(objIt, gameObjects.end());
	}
}