#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Engine/Entity/GameObjectNotifier.h"


namespace DeadFrame2D::Engine
{
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

	void GameObjectNotifier::RegisterOnNewComponentAddedHandler(std::function<void(const ComponentHandle<GameComponent>&)> handler, uintptr_t identifier)
	{
		OnNewComponentAdded.RegisterCallback(handler, identifier);
	}

	void GameObjectNotifier::RegisterOnComponentRemovedHandler(std::function<void(const ComponentHandle<GameComponent>&)> handler, uintptr_t identifier)
	{
		OnComponentRemoved.RegisterCallback(handler, identifier);
	}

	void GameObjectNotifier::DeregisterOnActiveStateChangedHandler(uintptr_t identifier)
	{
		OnActiveStateChanged.DeregisterCallback(identifier);
	}

	void GameObjectNotifier::DeregisterOnNewComponentAddedHandler(uintptr_t identifier)
	{
		OnNewComponentAdded.DeregisterCallback(identifier);
	}

	void GameObjectNotifier::DeregisterOnComponentRemovedHandler(uintptr_t identifier)
	{
		OnComponentRemoved.DeregisterCallback(identifier);
	}
}