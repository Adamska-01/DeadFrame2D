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

void GameObjectNotifier::RegisterOnActiveStateChangedHandler(std::function<void(GameObject*, bool)> handler, std::uintptr_t identifier)
{
	OnActiveStateChanged.RegisterCallback(handler, identifier);
}

void GameObjectNotifier::RegisterOnNewComponentAddedHandler(std::function<void(GameComponent*)> handler, std::uintptr_t identifier)
{
	OnNewComponentAdded.RegisterCallback(handler, identifier);
}

void GameObjectNotifier::DeregisterOnActiveStateChangedHandler(std::uintptr_t identifier)
{
	OnActiveStateChanged.DeregisterCallback(identifier);
}

void GameObjectNotifier::DeregisterOnNewComponentAddedHandler(std::uintptr_t identifier)
{
	OnNewComponentAdded.DeregisterCallback(identifier);
}