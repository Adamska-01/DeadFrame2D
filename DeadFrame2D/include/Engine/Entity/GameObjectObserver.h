#pragma once
#include "DF2D_API.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandleBase.h"
#include <vector>


namespace DeadFrame2D::Engine
{
	class GameObject;
	class GameComponent;

	template<typename T>
	class ComponentHandle;

	template<typename T>
	class ObjectHandle;


	class DF2D_API GameObjectObserver
	{
	private: 
		std::vector<ObjectHandleBase> allRegisteredGameObjects;


	protected:
		GameObjectObserver();

		virtual ~GameObjectObserver();


		void RegisterAllHandlers(const ObjectHandleBase& owner);

		void DeregisterAllHandlers(const ObjectHandleBase& owner);


		virtual void OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool isActive);

		virtual void OnNewComponentAddedHandler(const ComponentHandle<GameComponent>& comp);

		virtual void OnComponentRemovedHandler(const ComponentHandle<GameComponent>& comp);
	};
}