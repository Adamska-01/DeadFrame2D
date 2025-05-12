#include "GameObject.h"
#include "GameObjectObserver.h"
#include "Tools/Helpers/EventHelpers.h"


void GameObjectObserver::RegisterAllHandlers(GameObject* owner)
{
	owner->RegisterOnActiveStateChangedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnGameObjectActiveStateChangedHandler));
	owner->RegisterOnNewComponentAddedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnNewComponentAddedHandler));
}

void GameObjectObserver::DeregisterAllHandlers(GameObject* owner)
{
	owner->DeregisterOnActiveStateChangedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnGameObjectActiveStateChangedHandler));
	owner->DeregisterOnNewComponentAddedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnNewComponentAddedHandler));
}

void GameObjectObserver::OnGameObjectActiveStateChangedHandler(GameObject* obj, bool isActive)
{
}

void GameObjectObserver::OnNewComponentAddedHandler(GameComponent* comp)
{
}
