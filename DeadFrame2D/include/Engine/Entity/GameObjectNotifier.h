#pragma once
#include "DF2D_API.h"
#include "Engine/Entity/Handles/GameObject/IGameEntityHandle.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>


namespace DeadFrame2D::Engine
{
	class GameObject;
	class GameComponent;

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
		void RegisterOnActiveStateChangedHandler(std::function<void(ObjectHandle<GameObject>, bool)> handler, uintptr_t identifier);

		void RegisterOnNewComponentAddedHandler(std::function<void(const ComponentHandle<GameComponent>&)> handler, uintptr_t identifier);

		void RegisterOnComponentRemovedHandler(std::function<void(const ComponentHandle<GameComponent>&)> handler, uintptr_t identifier);


		void DeregisterOnActiveStateChangedHandler(uintptr_t identifier);

		void DeregisterOnNewComponentAddedHandler(uintptr_t identifier);

		void DeregisterOnComponentRemovedHandler(uintptr_t identifier);
	};
}