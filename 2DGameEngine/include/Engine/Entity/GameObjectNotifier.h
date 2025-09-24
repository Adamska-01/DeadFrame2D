#pragma once
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>


namespace DeadFrame2D::Engine
{
	class GameObject;
	class GameComponent;


	class GameObjectNotifier
	{
	protected:
		GameObjectNotifier();

		~GameObjectNotifier();

	
		DeadFrame2D::Utilities::MulticastDelegate<GameObject*, bool> OnActiveStateChanged;

		DeadFrame2D::Utilities::MulticastDelegate<GameComponent*> OnNewComponentAdded;


	public:
		void RegisterOnActiveStateChangedHandler(std::function<void(GameObject*, bool)> handler, uintptr_t identifier);
	
		void RegisterOnNewComponentAddedHandler(std::function<void(GameComponent*)> handler, uintptr_t identifier);


		void DeregisterOnActiveStateChangedHandler(uintptr_t identifier);
	
		void DeregisterOnNewComponentAddedHandler(uintptr_t identifier);
	};
}