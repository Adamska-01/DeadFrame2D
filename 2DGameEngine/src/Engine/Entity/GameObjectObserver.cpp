#include "Engine/Entity/GameObject.h"
#include "Engine/Entity/GameObjectObserver.h"
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
		for (auto obj : allRegisteredGameObjects)
		{
			DeregisterAllHandlers(obj);
		}

		allRegisteredGameObjects.clear();
	}

	void GameObjectObserver::RegisterAllHandlers(std::weak_ptr<GameObject> owner)
	{
		auto ownerPtr = owner.lock();
		if (ownerPtr == nullptr)
			return;

		auto it = std::remove_if(allRegisteredGameObjects.begin(), allRegisteredGameObjects.end(),
			[ownerPtr](const std::weak_ptr<GameObject>& other)
			{
				return other.lock().get() == ownerPtr.get();
			});

		if (it == allRegisteredGameObjects.end())
		{
			allRegisteredGameObjects.push_back(owner);
		}
	
		auto identifier = reinterpret_cast<uintptr_t>(this);

		ownerPtr->RegisterOnActiveStateChangedHandler(BindFunction(this, &GameObjectObserver::OnGameObjectActiveStateChangedHandler), identifier);
		ownerPtr->RegisterOnNewComponentAddedHandler(BindFunction(this, &GameObjectObserver::OnNewComponentAddedHandler), identifier);
	}

	void GameObjectObserver::DeregisterAllHandlers(std::weak_ptr<GameObject> owner)
	{
		auto ownerPtr = owner.lock();
		if (ownerPtr == nullptr)
			return;

		auto identifier = reinterpret_cast<uintptr_t>(this);

		ownerPtr->DeregisterOnActiveStateChangedHandler(identifier);
		ownerPtr->DeregisterOnNewComponentAddedHandler(identifier);
	}

	void GameObjectObserver::OnGameObjectActiveStateChangedHandler(GameObject* obj, bool isActive)
	{
	}

	void GameObjectObserver::OnNewComponentAddedHandler(GameComponent* comp)
	{
	}
}