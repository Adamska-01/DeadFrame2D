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

		DeadFrame2D::Utilities::MulticastDelegate<const ComponentHandle<GameComponent>&> OnNewComponentAdded;

		DeadFrame2D::Utilities::MulticastDelegate<const ComponentHandle<GameComponent>&> OnComponentRemoved;


	public:
		void RegisterOnActiveStateChangedHandler(const ComponentHandleBase& listener, std::function<void(ObjectHandle<GameObject>, bool)> handler);

		void RegisterOnNewComponentAddedHandler(const ComponentHandleBase& listener, std::function<void(const ComponentHandle<GameComponent>&)> handler);

		void RegisterOnComponentRemovedHandler(const ComponentHandleBase& listener, std::function<void(const ComponentHandle<GameComponent>&)> handler);


		void DeregisterOnActiveStateChangedHandler(const ComponentHandleBase& listener);

		void DeregisterOnNewComponentAddedHandler(const ComponentHandleBase& listener);

		void DeregisterOnComponentRemovedHandler(const ComponentHandleBase& listener);
	};
}