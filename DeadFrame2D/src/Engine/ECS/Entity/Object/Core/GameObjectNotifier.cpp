#include "Engine/ECS/Component/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/Entity/Object/Core/GameObjectNotifier.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"


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

	void GameObjectNotifier::RegisterOnGameObjectActiveStateChangedHandler(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>, bool)> handler)
	{
		OnActiveStateChanged.AddHandle(listener, handler);
	}

	void GameObjectNotifier::RegisterOnChildActiveStateChangedHandler(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>, bool)> handler)
	{
		OnChildActiveStateChanged.AddHandle(listener, handler);
	}

	void GameObjectNotifier::RegisterOnChildDestroyedHandler(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>)> handler)
	{
		OnChildDestroyed.AddHandle(listener, handler);
	}

	void GameObjectNotifier::RegisterOnNewComponentAddedHandler(const ComponentHandleBase& listener, std::function<void(const ComponentHandle<GameComponent>&)> handler)
	{
		OnNewComponentAdded.AddHandle(listener, handler);
	}

	void GameObjectNotifier::RegisterOnComponentRemovedHandler(const ComponentHandleBase& listener, std::function<void(const ComponentHandle<GameComponent>&)> handler)
	{
		OnComponentRemoved.AddHandle(listener, handler);
	}

	void GameObjectNotifier::RegisterOnChildGameObjectAdded(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>)> handler)
	{
		OnChildGameObjectAdded.AddHandle(listener, handler);
	}

	void GameObjectNotifier::RegisterOnParentGameObjectChanged(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>)> handler)
	{
		OnParentGameObjectChanged.AddHandle(listener, handler);
	}

	void GameObjectNotifier::DeregisterOnGameObjectActiveStateChangedHandler(const ComponentHandleBase& listener)
	{
		OnActiveStateChanged.RemoveByListener(&listener);
	}

	void GameObjectNotifier::DeregisterOnChildActiveStateChangedHandler(const ComponentHandleBase& listener)
	{
		OnChildActiveStateChanged.RemoveByListener(&listener);
	}

	void GameObjectNotifier::DeregisterOnChildDestroyedHandler(const ComponentHandleBase& listener)
	{
		OnChildDestroyed.RemoveByListener(&listener);
	}

	void GameObjectNotifier::DeregisterOnNewComponentAddedHandler(const ComponentHandleBase& listener)
	{
		OnNewComponentAdded.RemoveByListener(&listener);
	}

	void GameObjectNotifier::DeregisterOnComponentRemovedHandler(const ComponentHandleBase& listener)
	{
		OnComponentRemoved.RemoveByListener(&listener);
	}

	void GameObjectNotifier::DeregisterOnChildGameObjectAdded(const ComponentHandleBase& listener)
	{
		OnChildGameObjectAdded.RemoveByListener(&listener);
	}
	
	void GameObjectNotifier::DeregisterOnParentGameObjectChanged(const ComponentHandleBase& listener)
	{
		OnParentGameObjectChanged.RemoveByListener(&listener);
	}
}