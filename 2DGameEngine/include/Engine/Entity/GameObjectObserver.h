#pragma once
#include "DF2D_API.h"
#include <memory>
#include <vector>


namespace DeadFrame2D::Engine
{
	class GameObject;
	class GameComponent;


	class DF2D_API GameObjectObserver
	{
	private: 
		std::vector<std::weak_ptr<GameObject>> allRegisteredGameObjects;


	protected:
		GameObjectObserver();

		virtual ~GameObjectObserver();


		void RegisterAllHandlers(std::weak_ptr<GameObject> owner);

		void DeregisterAllHandlers(std::weak_ptr<GameObject> owner);


		virtual void OnGameObjectActiveStateChangedHandler(GameObject* obj, bool isActive);

		virtual void OnNewComponentAddedHandler(GameComponent* comp);
	};
}