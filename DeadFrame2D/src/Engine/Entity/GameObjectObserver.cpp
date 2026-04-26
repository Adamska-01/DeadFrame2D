#include "Engine/ECS/Component/GameComponent.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Engine/Entity/GameObject.h"
#include "Engine/Entity/GameObjectObserver.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandle.h"
#include "Utilities/Helpers/Events/EventHelpers.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Utilities;


	GameObjectObserver::GameObjectObserver()
	{
		allRegisteredGameObjects.clear();
	}

	GameObjectObserver::~GameObjectObserver()
	{
		for (const auto& obj : allRegisteredGameObjects)
		{
			DeregisterAllHandlers(obj);
		}

		allRegisteredGameObjects.clear();
	}

	void GameObjectObserver::RegisterAllHandlers(const ObjectHandleBase& targetObj)
	{
		if (targetObj == nullptr)
			return;

		auto it = std::remove_if(
			allRegisteredGameObjects.begin(),
			allRegisteredGameObjects.end(),
			[targetObj](const auto& other)
			{
				return other == targetObj;
			});

		if (it == allRegisteredGameObjects.end())
		{
			allRegisteredGameObjects.push_back(targetObj);
		}
	
		auto typedHandle = ObjectHandle<GameObject>::From(targetObj);

		typedHandle->RegisterOnGameObjectActiveStateChangedHandler(GetHandle(), EventHelpers::BindFunction(this, &GameObjectObserver::OnGameObjectActiveStateChangedHandler));
		typedHandle->RegisterOnChildActiveStateChangedHandler(GetHandle(), EventHelpers::BindFunction(this, &GameObjectObserver::OnChildActiveStateChangedHandler));
		typedHandle->RegisterOnChildDestroyedHandler(GetHandle(), EventHelpers::BindFunction(this, &GameObjectObserver::OnChildDestroyedHandler));
		typedHandle->RegisterOnNewComponentAddedHandler(GetHandle(), EventHelpers::BindFunction(this, &GameObjectObserver::OnNewComponentAddedHandler));
		typedHandle->RegisterOnComponentRemovedHandler(GetHandle(), EventHelpers::BindFunction(this, &GameObjectObserver::OnComponentRemovedHandler));
		typedHandle->RegisterOnChildGameObjectAdded(GetHandle(), EventHelpers::BindFunction(this, &GameObjectObserver::OnChildGameObjectAddedHandler));
		typedHandle->RegisterOnParentGameObjectChanged(GetHandle(), EventHelpers::BindFunction(this, &GameObjectObserver::OnParentGameObjectChangedHandler));
	}

	void GameObjectObserver::DeregisterAllHandlers(const ObjectHandleBase& targetObj)
	{
		if (targetObj == nullptr)
			return;

		auto typedHandle = ObjectHandle<GameObject>::From(targetObj);

		typedHandle->DeregisterOnGameObjectActiveStateChangedHandler(GetHandle());
		typedHandle->DeregisterOnChildActiveStateChangedHandler(GetHandle());
		typedHandle->DeregisterOnChildDestroyedHandler(GetHandle());
		typedHandle->DeregisterOnNewComponentAddedHandler(GetHandle());
		typedHandle->DeregisterOnComponentRemovedHandler(GetHandle());
		typedHandle->DeregisterOnChildGameObjectAdded(GetHandle());
		typedHandle->DeregisterOnParentGameObjectChanged(GetHandle());
	}


	void GameObjectObserver::OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool isActive)
	{
	}

	void GameObjectObserver::OnChildActiveStateChangedHandler(const ObjectHandle<GameObject>& child, bool isActive)
	{
	}

	void GameObjectObserver::OnChildDestroyedHandler(const ObjectHandle<GameObject>& destroyedObj)
	{
	}

	void GameObjectObserver::OnNewComponentAddedHandler(const ComponentHandle<GameComponent>& comp)
	{
	}

	void GameObjectObserver::OnComponentRemovedHandler(const ComponentHandle<GameComponent>& comp)
	{
	}

	void GameObjectObserver::OnChildGameObjectAddedHandler(const ObjectHandle<GameObject>& obj)
	{
	}

	void GameObjectObserver::OnParentGameObjectChangedHandler(const ObjectHandle<GameObject>& obj)
	{
	}
}