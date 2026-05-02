#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/ComponentContext.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandleBase.h"
#include <vector>


namespace DF2D::Engine
{
	class GameObject;
	class GameComponent;

	template<typename T>
	class ComponentHandle;

	template<typename T>
	class ObjectHandle;


	class DF2D_API GameObjectObserver : public ComponentContext
	{
		friend class ComponentBucket;


	private: 
		std::vector<ObjectHandleBase> allRegisteredGameObjects;


	protected:
		GameObjectObserver();

		virtual ~GameObjectObserver();


		void RegisterAllHandlers(const ObjectHandleBase& targetObj);

		void DeregisterAllHandlers(const ObjectHandleBase& targetObj);


		virtual void OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool isActive);

		virtual void OnChildActiveStateChangedHandler(const ObjectHandle<GameObject>& child, bool isActive);

		virtual void OnChildDestroyedHandler(const ObjectHandle<GameObject>& destroyedObj);

		virtual void OnNewComponentAddedHandler(const ComponentHandle<GameComponent>& comp);

		virtual void OnComponentRemovedHandler(const ComponentHandle<GameComponent>& comp);

		virtual void OnChildGameObjectAddedHandler(const ObjectHandle<GameObject>& obj);

		virtual void OnParentGameObjectChangedHandler(const ObjectHandle<GameObject>& obj);
	};
}