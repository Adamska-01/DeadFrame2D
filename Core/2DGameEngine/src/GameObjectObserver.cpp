#include "GameObject.h"
#include "GameObjectObserver.h"
#include "Tools/Helpers/EventHelpers.h"


GameObjectObserver::GameObjectObserver()
{
	allRegisteredGameObjects.clear();
}

GameObjectObserver::~GameObjectObserver()
{
	for (auto obj : allRegisteredGameObjects)
	{
		if (obj == nullptr || obj->IsMarkedForDestruction())
			continue;

		DeregisterAllHandlers(obj);
	}

	allRegisteredGameObjects.clear();
}

void GameObjectObserver::RegisterAllHandlers(GameObject* owner)
{
	auto it = std::remove(allRegisteredGameObjects.begin(), allRegisteredGameObjects.end(), owner);
	if (it == allRegisteredGameObjects.end())
	{
		allRegisteredGameObjects.push_back(owner);
	}

	owner->RegisterOnActiveStateChangedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnGameObjectActiveStateChangedHandler));
	owner->RegisterOnNewComponentAddedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnNewComponentAddedHandler));
}

void GameObjectObserver::DeregisterAllHandlers(GameObject* owner)
{
	auto it = std::remove(allRegisteredGameObjects.begin(), allRegisteredGameObjects.end(), owner);
	if (it != allRegisteredGameObjects.end())
	{
		allRegisteredGameObjects.erase(it, allRegisteredGameObjects.end());
	}

	owner->DeregisterOnActiveStateChangedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnGameObjectActiveStateChangedHandler));
	owner->DeregisterOnNewComponentAddedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnNewComponentAddedHandler));
}

void GameObjectObserver::OnGameObjectActiveStateChangedHandler(GameObject* obj, bool isActive)
{
}

void GameObjectObserver::OnNewComponentAddedHandler(GameComponent* comp)
{
}
