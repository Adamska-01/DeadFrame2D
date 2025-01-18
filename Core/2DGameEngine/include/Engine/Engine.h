#pragma once
#include "Management/GameObjectRegistry.h"
#include "SubSystems/Events/EventManager.h"
#include "SubSystems/SubSystems.h"
#include "Tools/FrameTimer.h"


class Engine
{
private:
	std::unique_ptr<SubSystems> engineSubSystems;

	std::unique_ptr<GameObjectRegistry> gameObjectRegistry;

	EventManager eventManager;

	FrameTimer ft;


	void Update(float deltaTime);

	void Draw();


public:
	Engine();
	
	~Engine() = default;


	std::optional<int> Run();
};