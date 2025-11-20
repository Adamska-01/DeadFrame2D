#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Engine/Entity/GameObject.h"
#include "Engine/Entity/GameObjectNotifier.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandle.h"


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

	void GameObjectNotifier::RegisterOnActiveStateChangedHandler(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>, bool)> handler)
	{
		OnActiveStateChanged.AddHandle(listener, handler);
	}

	void GameObjectNotifier::RegisterOnNewComponentAddedHandler(const ComponentHandleBase& listener, std::function<void(const ComponentHandle<GameComponent>&)> handler)
	{
		OnNewComponentAdded.AddHandle(listener, handler);
	}

	void GameObjectNotifier::RegisterOnComponentRemovedHandler(const ComponentHandleBase& listener, std::function<void(const ComponentHandle<GameComponent>&)> handler)
	{
		OnComponentRemoved.AddHandle(listener, handler);
	}

	void GameObjectNotifier::DeregisterOnActiveStateChangedHandler(const ComponentHandleBase& listener)
	{
		OnActiveStateChanged.RemoveByListener(&listener);
	}

	void GameObjectNotifier::DeregisterOnNewComponentAddedHandler(const ComponentHandleBase& listener)
	{
		OnNewComponentAdded.RemoveByListener(&listener);
	}

	void GameObjectNotifier::DeregisterOnComponentRemovedHandler(const ComponentHandleBase& listener)
	{
		OnComponentRemoved.RemoveByListener(&listener);
	}
}