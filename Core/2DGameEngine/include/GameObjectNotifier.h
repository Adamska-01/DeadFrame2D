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
	void RegisterOnActiveStateChangedHandler(std::function<void(GameObject*, bool)> handler);
	
	void RegisterOnNewComponentAddedHandler(std::function<void(GameComponent*)> handler);


	void DeregisterOnActiveStateChangedHandler(std::function<void(GameObject*, bool)> handler);
	
	void DeregisterOnNewComponentAddedHandler(std::function<void(GameComponent*)> handler);
};