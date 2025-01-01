#pragma once
#include <SubSystems/Events/EventManager.h>
#include <Utility/FrameTimer.h>
#include <vector>


class SubSystems;


class Application
{
private:
	SubSystems* engineSubSystems;

	EventManager eventManager;

	FrameTimer ft;


	void Update();

	void Draw();

	void Clean();


public:
	Application();

	~Application();


	int Run();
};