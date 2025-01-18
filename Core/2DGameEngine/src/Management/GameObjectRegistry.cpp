#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Management/GameObjectRegistry.h"
#include <EventSystem/EventDispatcher.h>
#include <Tools/Helpers/EventHelpers.h>


GameObjectRegistry::GameObjectRegistry()
{
	gameObjects.clear();

	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &GameObjectRegistry::GameObjectCreatedHandler));
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &GameObjectRegistry::GameObjectDestroyedHandler));
}

GameObjectRegistry::~GameObjectRegistry()
{
	gameObjects.clear();

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

	gameObjects.push_back(std::move(gameObjEvent->gameObjectCreated));
}

void GameObjectRegistry::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

	if (gameObjEvent == nullptr || gameObjEvent->gameObjectDestroyed == nullptr)
		return;

	auto target = gameObjEvent->gameObjectDestroyed;

	auto it = std::remove_if(
		gameObjects.begin(),
		gameObjects.end(),
		[target](std::shared_ptr<GameObject>& gameObject)
		{
			return gameObject.get() == target;
		});

	if (it != gameObjects.end())
	{
		gameObjects.erase(it, gameObjects.end());
	}
}