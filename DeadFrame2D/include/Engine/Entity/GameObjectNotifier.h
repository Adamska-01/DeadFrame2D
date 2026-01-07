#pragma once
#include "DF2D_API.h"
#include "Engine/Entity/Handles/GameObject/IGameEntityHandle.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>


namespace DeadFrame2D::Engine
{
	class GameObject;
	class GameComponent;
	class ComponentHandleBase;

	template<typename T>
	class ComponentHandle;

	template<typename T>
	class ObjectHandle;


	class DF2D_API GameObjectNotifier : public IGameEntityHandle
	{
	protected:
		GameObjectNotifier();

		~GameObjectNotifier();


		DeadFrame2D::Utilities::MulticastDelegate<ObjectHandle<GameObject>, bool> OnActiveStateChanged;

		DeadFrame2D::Utilities::MulticastDelegate<ObjectHandle<GameObject>, bool> OnChildActiveStateChanged;

		DeadFrame2D::Utilities::MulticastDelegate<const ComponentHandle<GameComponent>&> OnNewComponentAdded;

		DeadFrame2D::Utilities::MulticastDelegate<const ComponentHandle<GameComponent>&> OnComponentRemoved;

		DeadFrame2D::Utilities::MulticastDelegate<ObjectHandle<GameObject>> OnChildGameObjectAdded;

		DeadFrame2D::Utilities::MulticastDelegate<ObjectHandle<GameObject>> OnParentGameObjectChanged;


	public:
		void RegisterOnGameObjectActiveStateChangedHandler(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>, bool)> handler);

		void RegisterOnChildActiveStateChangedHandler(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>, bool)> handler);

		void RegisterOnNewComponentAddedHandler(const ComponentHandleBase& listener, std::function<void(const ComponentHandle<GameComponent>&)> handler);

		void RegisterOnComponentRemovedHandler(const ComponentHandleBase& listener, std::function<void(const ComponentHandle<GameComponent>&)> handler);

		void RegisterOnChildGameObjectAdded(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>)> handler);
		
		void RegisterOnParentGameObjectChanged(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>)> handler);


		void DeregisterOnGameObjectActiveStateChangedHandler(const ComponentHandleBase& listener);

		void DeregisterOnChildActiveStateChangedHandler(const ComponentHandleBase& listener);

		void DeregisterOnNewComponentAddedHandler(const ComponentHandleBase& listener);

		void DeregisterOnComponentRemovedHandler(const ComponentHandleBase& listener);

		void DeregisterOnChildGameObjectAdded(const ComponentHandleBase& listener);
		
		void DeregisterOnParentGameObjectChanged(const ComponentHandleBase& listener);
	};
}