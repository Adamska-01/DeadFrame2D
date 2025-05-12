#pragma once
#include <vector>


class GameObject;
class GameComponent;


class GameObjectObserver 
{
private: 
	std::vector<GameObject*> allRegisteredGameObjects;


protected:
	GameObjectObserver();
	
	virtual ~GameObjectObserver();


	void RegisterAllHandlers(GameObject* owner);

	void DeregisterAllHandlers(GameObject* owner);


	virtual void OnGameObjectActiveStateChangedHandler(GameObject* obj, bool isActive);
	
	virtual void OnNewComponentAddedHandler(GameComponent* comp);
};