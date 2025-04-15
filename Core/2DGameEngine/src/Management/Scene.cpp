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

	collisionHandler = CollisionHandler();

	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectCreatedHandler));
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &Scene::GameObjectDestroyedHandler));
}

Scene::~Scene()
{
	gameObjects.clear();
	colliders.clear();

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

	if (collider == nullptr)
		return;

	colliders.push_back(collider);

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
		gameObjects[i]->Update(deltaTime);
	}

	for (size_t i = 0; i < colliderSize; ++i)
	{
		for (size_t j = i + 1; j < colliderSize; ++j)
		{
			auto colliderA = colliders[i];
			auto colliderB = colliders[j];

			colliderA->Accept(collisionHandler, colliderB);
			colliderB->Accept(collisionHandler, colliderA);
		}
	}
}

void Scene::Draw()
{
	Renderer::ClearBuffer();

	for (const auto& obj : gameObjects)
	{
		obj->Draw();
	}

	Renderer::PresentBuffer();
}