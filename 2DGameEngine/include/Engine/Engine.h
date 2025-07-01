#pragma once
#include "EventSystem/DispatchableEvent.h"
#include "Management/SceneManager.h"
#include "SubSystems/Events/EventManager.h"
#include "SubSystems/SubSystems.h"
#include "Tools/FrameTimer.h"
#include <Models/Engine/EngineConfig.h>


class Engine
{
protected:
	std::unique_ptr<SubSystems> engineSubSystems;

	std::unique_ptr<SceneManager> sceneManager;

	EventManager eventManager;

	FrameTimer frameTimer;

	EngineConfig engineConfig;


public:
	Engine();
	
	virtual ~Engine() = default;


	std::optional<int> Run();
};