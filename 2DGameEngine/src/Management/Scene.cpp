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

	Exit();

	auto identifier = reinterpret_cast<uintptr_t>(this);
	
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectCreatedHandler), identifier);
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectDestroyedHandler), identifier);
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(ChildGameObjectAddedEvent)), EventHelpers::BindFunction(this, &Scene::ChildGameObjectAddedHandler), identifier);
}

Scene::~Scene()
{
	Exit();

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
	objectsPendingCreation.push_back(target);

	if (isRunning)
	{
		target->Init();
		target->Start();

		return;
	}

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

	childAddedPendingAction.push_back(gameObjEvent->childGameObject);
	
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
	auto creationPendingSize = objectsPendingCreation.size();

	if (creationPendingSize > 0)
	{
		for (size_t i = 0; i < creationPendingSize; ++i)
		{
			auto& obj = objectsPendingCreation[i];

			gameObjectParents.push_back(obj);

			std::stable_partition(
				gameObjectParents.begin(),
				gameObjectParents.end(),
				[](const std::shared_ptr<GameObject>& obj)
				{
					return obj->GetComponent<Canvas>() == nullptr;
				});
		}

		objectsPendingCreation.clear();
	}
	
	auto childAddedPendingActionSize = childAddedPendingAction.size();

	if (childAddedPendingActionSize > 0)
	{
		for (size_t i = 0; i < childAddedPendingActionSize; ++i)
		{
			auto& childObj = childAddedPendingAction[i];

			gameObjectParents.erase(
				std::remove_if(
					gameObjectParents.begin(),
					gameObjectParents.end(),
					[&childObj](const auto& obj)
					{
						return obj.get() == childObj.lock().get();
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

		childAddedPendingAction.clear();
	}

	auto parentsSize = gameObjectParents.size();

	for (size_t i = 0; i < parentsSize; ++i)
	{
		auto& obj = gameObjectParents[i];
		
		if (!obj->IsActive())
			continue;

		obj->Update(deltaTime);
	}
}

void Scene::LateUpdate(float deltaTime)
{
	auto parentsSize = gameObjectParents.size();

	for (size_t i = 0; i < parentsSize; ++i)
	{
		auto& obj = gameObjectParents[i];
		
		if (!obj->IsActive())
			continue;

		obj->LateUpdate(deltaTime);
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
	objectsPendingCreation.clear();
	objectsPendingDestroy.clear();
}