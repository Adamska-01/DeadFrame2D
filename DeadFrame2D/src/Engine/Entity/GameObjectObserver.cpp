#include "Engine/Components/GameComponent.h"
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

		typedHandle->RegisterOnActiveStateChangedHandler(GetHandle(), EventHelpers::BindFunction(this, &GameObjectObserver::OnGameObjectActiveStateChangedHandler));
		typedHandle->RegisterOnNewComponentAddedHandler(GetHandle(), EventHelpers::BindFunction(this, &GameObjectObserver::OnNewComponentAddedHandler));
	}

	void GameObjectObserver::DeregisterAllHandlers(const ObjectHandleBase& targetObj)
	{
		if (targetObj == nullptr)
			return;

		auto typedHandle = ObjectHandle<GameObject>::From(targetObj);

		typedHandle->DeregisterOnActiveStateChangedHandler(GetHandle());
		typedHandle->DeregisterOnNewComponentAddedHandler(GetHandle());
	}

	void GameObjectObserver::OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool isActive)
	{
	}

	void GameObjectObserver::OnNewComponentAddedHandler(const ComponentHandle<GameComponent>& comp)
	{
	}

	void GameObjectObserver::OnComponentRemovedHandler(const ComponentHandle<GameComponent>& comp)
	{
	}
}