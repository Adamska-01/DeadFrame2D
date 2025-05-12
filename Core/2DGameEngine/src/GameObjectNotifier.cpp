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

void GameObjectNotifier::RegisterOnActiveStateChangedHandler(std::function<void(GameObject*, bool)> handler)
{
	OnActiveStateChanged += handler;
}

void GameObjectNotifier::RegisterOnNewComponentAddedHandler(std::function<void(GameComponent*)> handler)
{
	OnNewComponentAdded += handler;
}

void GameObjectNotifier::DeregisterOnActiveStateChangedHandler(std::function<void(GameObject*, bool)> handler)
{
	OnActiveStateChanged -= handler;
}

void GameObjectNotifier::DeregisterOnNewComponentAddedHandler(std::function<void(GameComponent*)> handler)
{
	OnNewComponentAdded -= handler;
}