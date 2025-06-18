#include "Components/UI/Canvas.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/GameObjectEvents/ChildGameObjectAddedEvent.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Management/Scene.h"
#include "Tools/Helpers/EventHelpers.h"
#include <algorithm>


Scene::Scene()
{
	isRunning = false;

	gameObjects.clear();
	gameObjectsToInitialize.clear();
	objectsPendingDestroy.clear();

	auto identifier = reinterpret_cast<uintptr_t>(this);
	
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectCreatedHandler), identifier);
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectDestroyedHandler), identifier);
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(ChildGameObjectAddedEvent)), EventHelpers::BindFunction(this, &Scene::ChildGameObjectAddedHandler), identifier);
}

Scene::~Scene()
{
	gameObjects.clear();
	gameObjectsToInitialize.clear();
	objectsPendingDestroy.clear();

	auto identifier = reinterpret_cast<uintptr_t>(this);
	
	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), identifier);
	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), identifier);
	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(ChildGameObjectAddedEvent)), identifier);
}

void Scene::GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectCreatedEvent>(dispatchableEvent);

	if (gameObjEvent == nullptr || gameObjEvent->gameObjectCreated == nullptr)
		return;

	auto target = gameObjEvent->gameObjectCreated;

	gameObjects.push_back(target);
	gameObjectParents.push_back(target);

	std::stable_partition(
		gameObjectParents.begin(),
		gameObjectParents.end(),
		[](const std::shared_ptr<GameObject>& obj)
		{
			return obj->GetComponent<Canvas>() == nullptr;
		});

	if (isRunning)
	{
		target->Init();
		target->Start();

		return;
	}

	// Store for late-initialization
	gameObjectsToInitialize.push_back(target);
}

void Scene::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

	if (!gameObjEvent || gameObjEvent->gameObjectDestroyed.lock() == nullptr)
		return;

	auto toDestroyPtr = gameObjEvent->gameObjectDestroyed;

	// Mark for destruction
	objectsPendingDestroy.push_back(toDestroyPtr);
}

void Scene::ChildGameObjectAddedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<ChildGameObjectAddedEvent>(dispatchableEvent);

	if (!gameObjEvent || gameObjEvent->childGameObject.lock() == nullptr)
		return;

	auto childGameObject = gameObjEvent->childGameObject;

	gameObjectParents.erase(
		std::remove_if(
			gameObjectParents.begin(),
			gameObjectParents.end(),
			[&childGameObject](const auto& obj)
			{
				return obj.get() == childGameObject.lock().get();
			}),
		gameObjectParents.end());

	std::stable_partition(
		gameObjectParents.begin(),
		gameObjectParents.end(),
		[](const std::shared_ptr<GameObject>& obj)
		{
			return obj->GetComponent<Canvas>() == nullptr;
		});
}

void Scene::CleanupDestroyedObjects()
{
	if (objectsPendingDestroy.empty())
		return;

	for (const auto& weakTarget : objectsPendingDestroy)
	{
		auto targetPtr = weakTarget.lock();
		if (!targetPtr)
			continue;

		const auto matchesTarget = [&targetPtr](const std::shared_ptr<GameObject>& obj)
			{
				return obj.get() == targetPtr.get();
			};

		gameObjects.erase(
			std::remove_if(
				gameObjects.begin(), 
				gameObjects.end(), 
				matchesTarget),
			gameObjects.end());

		if (targetPtr->GetParent().expired())
		{
			gameObjectParents.erase(
				std::remove_if(
					gameObjectParents.begin(), 
					gameObjectParents.end(), 
					matchesTarget),
				gameObjectParents.end());
		}
	}

	objectsPendingDestroy.clear();
}

void Scene::Init()
{
	// Some game objects might create other game objects in their Init function.
	// However, we want to ensure that only the game objects in the current
	// initialization queue are considered part of this initial phase. 
	// All the game objects created after this step are initalised at creation-time.
	auto currentToInitialize = gameObjectsToInitialize;
	gameObjectsToInitialize.clear();

	for (const auto& toInitialize : currentToInitialize)
	{
		auto toInitPtr = toInitialize.lock();

		if (toInitPtr == nullptr)
			continue;

		toInitPtr->Init();
	}

	for (const auto& toInitialize : currentToInitialize)
	{
		auto toInitPtr = toInitialize.lock();

		if (toInitPtr == nullptr)
			continue;

		toInitPtr->Start();
	}

	isRunning = true;

	for (const auto& toInitialize : gameObjectsToInitialize)
	{
		auto toInitPtr = toInitialize.lock();

		if (toInitPtr == nullptr)
			continue;

		toInitPtr->Init();
	}

	for (const auto& toInitialize : gameObjectsToInitialize)
	{
		auto toInitPtr = toInitialize.lock();

		if (toInitPtr == nullptr)
			continue;

		toInitPtr->Start();
	}

	currentToInitialize.clear();
	gameObjectsToInitialize.clear();
}

void Scene::Update(float deltaTime)
{
	auto gameobjectSize = gameObjectParents.size();

	for (const auto& obj : gameObjectParents)
	{
		if (!obj->IsActive())
			continue;

		obj->Update(deltaTime);
	}

	CleanupDestroyedObjects();
}

void Scene::Draw()
{
	for (const auto& obj : gameObjectParents)
	{
		if (!obj->IsActive())
			continue;

		obj->Draw();
	}
}

void Scene::Exit()
{
	gameObjects.clear();
	gameObjectParents.clear();
	gameObjectsToInitialize.clear();
	objectsPendingDestroy.clear();
}