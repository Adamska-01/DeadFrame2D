#pragma once
#include <memory>
#include <vector>


class GameObject;
class GameComponent;


class GameObjectObserver 
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