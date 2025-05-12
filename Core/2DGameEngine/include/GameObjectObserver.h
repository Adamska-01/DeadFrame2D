#pragma once


class GameObject;
class GameComponent;


class GameObjectObserver 
{
protected:
	GameObjectObserver() = default;
	
	virtual ~GameObjectObserver() = default;


	void RegisterAllHandlers(GameObject* owner);

	void DeregisterAllHandlers(GameObject* owner);


	virtual void OnGameObjectActiveStateChangedHandler(GameObject* obj, bool isActive);
	
	virtual void OnNewComponentAddedHandler(GameComponent* comp);
};