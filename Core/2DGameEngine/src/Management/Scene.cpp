#include "Components/Collisions/Collider2D.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Management/Scene.h"
#include "SubSystems/Renderer.h"
#include "Tools/Helpers/EventHelpers.h"


Scene::Scene()
{
	isRunning = false;

	gameObjects.clear();
	colliders.clear();
	gameObjectsToInitialize.clear();
	objectsPendingDestroy.clear();

	collisionHandler = CollisionHandler();

	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectCreatedHandler));
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectDestroyedHandler));
}

Scene::~Scene()
{
	gameObjects.clear();
	colliders.clear();
	gameObjectsToInitialize.clear();
	objectsPendingDestroy.clear();

	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectCreatedHandler));
	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectDestroyedHandler));
}

void Scene::GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectCreatedEvent>(dispatchableEvent);

	if (gameObjEvent == nullptr || gameObjEvent->gameObjectCreated == nullptr)
		return;

	auto target = gameObjEvent->gameObjectCreated;

	gameObjects.push_back(target);

	auto collider = gameObjects.back()->GetComponent<Collider2D>();

	if (collider != nullptr)
	{
		colliders.push_back(collider);
	}

	if (isRunning)
	{
		target->Init();

		return;
	}

	// Store for late-initialization
	gameObjectsToInitialize.push_back(target);
}

void Scene::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

	if (!gameObjEvent || !gameObjEvent->gameObjectDestroyed)
		return;

	// Mark for destruction
	objectsPendingDestroy.push_back(gameObjEvent->gameObjectDestroyed);
}

void Scene::CleanupDestroyedObjects()
{
	if (objectsPendingDestroy.empty())
		return;

	for (const auto& target : objectsPendingDestroy)
	{
		if (!target)
			continue;

		// Remove collider if it exists
		auto targetCollider = target->GetComponent<Collider2D>();
		if (targetCollider != nullptr)
		{
			auto colliderIt = std::remove(
				colliders.begin(), 
				colliders.end(), 
				targetCollider);
			
			colliders.erase(colliderIt, colliders.end());
		}

		// Remove GameObject
		auto objIt = std::remove_if(
			gameObjects.begin(), 
			gameObjects.end(),
			[target](const std::shared_ptr<GameObject>& obj)
			{
				return obj.get() == target;
			});
		
		gameObjects.erase(objIt, gameObjects.end());
	}

	objectsPendingDestroy.clear();
}

void Scene::Exit()
{
	gameObjects.clear();
	colliders.clear();
	gameObjectsToInitialize.clear();
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

	isRunning = true;

	for (const auto& toInitialize : gameObjectsToInitialize)
	{
		auto toInitPtr = toInitialize.lock();

		if (toInitPtr == nullptr)
			continue;

		toInitPtr->Init();
	}

	currentToInitialize.clear();
	gameObjectsToInitialize.clear();
}

void Scene::Update(float deltaTime)
{
	auto gameobjectSize = gameObjects.size();
	auto colliderSize = colliders.size();

	for (size_t i = 0; i < gameobjectSize; i++)
	{
		if (!gameObjects[i]->IsActive())
			continue;

		gameObjects[i]->Update(deltaTime);
	}

	for (size_t i = 0; i < colliderSize; ++i)
	{
		for (size_t j = i + 1; j < colliderSize; ++j)
		{
			if (!colliders[i]->GetGameObject()->IsActive()
				|| !colliders[j]->GetGameObject()->IsActive())
				continue;

			auto colliderA = colliders[i];
			auto colliderB = colliders[j];

			colliderA->Accept(collisionHandler, colliderB);
			colliderB->Accept(collisionHandler, colliderA);
		}
	}

	CleanupDestroyedObjects();
}

void Scene::Draw()
{
	Renderer::ClearBuffer();

	for (const auto& obj : gameObjects)
	{
		if (!obj->IsActive())
			continue;

		obj->Draw();
	}

	Renderer::PresentBuffer();
}