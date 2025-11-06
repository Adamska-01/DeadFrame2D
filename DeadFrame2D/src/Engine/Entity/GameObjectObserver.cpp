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

	void GameObjectObserver::RegisterAllHandlers(const ObjectHandleBase& owner)
	{
		if (owner == nullptr)
			return;

		auto it = std::remove_if(
			allRegisteredGameObjects.begin(),
			allRegisteredGameObjects.end(),
			[owner](const auto& other)
			{
				return other == owner;
			});

		if (it == allRegisteredGameObjects.end())
		{
			allRegisteredGameObjects.push_back(owner);
		}
	
		auto identifier = reinterpret_cast<uintptr_t>(this);
		auto typedOwner = ObjectHandle<GameObject>::From(owner);

		typedOwner->RegisterOnActiveStateChangedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnGameObjectActiveStateChangedHandler), identifier);
		typedOwner->RegisterOnNewComponentAddedHandler(EventHelpers::BindFunction(this, &GameObjectObserver::OnNewComponentAddedHandler), identifier);
	}

	void GameObjectObserver::DeregisterAllHandlers(const ObjectHandleBase& owner)
	{
		if (owner == nullptr)
			return;

		auto identifier = reinterpret_cast<uintptr_t>(this);
		auto typedOwner = ObjectHandle<GameObject>::From(owner);

		typedOwner->DeregisterOnActiveStateChangedHandler(identifier);
		typedOwner->DeregisterOnNewComponentAddedHandler(identifier);
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