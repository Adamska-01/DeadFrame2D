#include "GameObjectNotifier.h"


GameObjectNotifier::GameObjectNotifier()
{
	OnActiveStateChanged.Clear();
	OnNewComponentAdded.Clear();
}

GameObjectNotifier ::~GameObjectNotifier()
{
	OnActiveStateChanged.Clear();
	OnNewComponentAdded.Clear();
}

void GameObjectNotifier::RegisterOnActiveStateChangedHandler(std::function<void(GameObject*, bool)> handler, uintptr_t identifier)
{
	OnActiveStateChanged.RegisterCallback(handler, identifier);
}

void GameObjectNotifier::RegisterOnNewComponentAddedHandler(std::function<void(GameComponent*)> handler, uintptr_t identifier)
{
	OnNewComponentAdded.RegisterCallback(handler, identifier);
}

void GameObjectNotifier::DeregisterOnActiveStateChangedHandler(uintptr_t identifier)
{
	OnActiveStateChanged.DeregisterCallback(identifier);
}

void GameObjectNotifier::DeregisterOnNewComponentAddedHandler(uintptr_t identifier)
{
	OnNewComponentAdded.DeregisterCallback(identifier);
}