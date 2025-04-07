#pragma once
#include "EventSystem/DispatchableEvent.h"
#include "Management/GameObjectRegistry.h"
#include "SubSystems/Events/EventManager.h"
#include "SubSystems/SubSystems.h"
#include "Tools/FrameTimer.h"


class Engine
{
private:
	bool isRunning;

	std::vector<std::shared_ptr<GameObject>> gameObjectsToInitialize;

	
	void GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


protected:
	std::unique_ptr<SubSystems> engineSubSystems;

	std::unique_ptr<GameObjectRegistry> gameObjectRegistry;

	EventManager eventManager;

	FrameTimer ft;


	virtual void Update(float deltaTime);

	virtual void Draw();


public:
	Engine();
	
	virtual ~Engine() = default;


	std::optional<int> Run();
};