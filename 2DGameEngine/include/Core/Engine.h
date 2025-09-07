#pragma once
#include "CoreEvents/EventManager.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/SceneSystem/SceneManager.h"
#include "Framerate/FrameTimer.h"
#include "SubSystems/SubSystems.h"
#include <Models/Engine/EngineConfig.h>


class Engine
{
private:
	std::optional<int> RenderSplashScreen();


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