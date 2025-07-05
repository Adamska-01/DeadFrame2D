#pragma once
#include "Tools/MulticastDelegate.h"
#include <functional>


class GameObject;
class GameComponent;


class GameObjectNotifier
{
protected:
	GameObjectNotifier();

	~GameObjectNotifier();

	
	MulticastDelegate<GameObject*, bool> OnActiveStateChanged;

	MulticastDelegate<GameComponent*> OnNewComponentAdded;


public:
	void RegisterOnActiveStateChangedHandler(std::function<void(GameObject*, bool)> handler, uintptr_t identifier);
	
	void RegisterOnNewComponentAddedHandler(std::function<void(GameComponent*)> handler, uintptr_t identifier);


	void DeregisterOnActiveStateChangedHandler(uintptr_t identifier);
	
	void DeregisterOnNewComponentAddedHandler(uintptr_t identifier);
};