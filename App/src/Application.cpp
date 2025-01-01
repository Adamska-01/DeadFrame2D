#include "Application.h"
#include <SubSystems/AudioManager.h>
#include <SubSystems/Input.h>
#include <SubSystems/Renderer.h>
#include <SubSystems/SubSystems.h>
#include <SubSystems/TextureManager.h>
#include <SubSystems/UIManager.h>


Application::Application()
{
	engineSubSystems = new SubSystems();

	engineSubSystems->InitializeSubSystems();
}

Application::~Application()
{
	Clean();
}

int Application::Run()
{
	while (true)
	{
		ft.StartClock();
		
		//Looks for messages and return optional if QUIT
		if (const auto ecode = eventManager.ProcessEvents())
			return *ecode;

		Update();
		Draw();

		//FPS and delay
		ft.EndClock();
		ft.DelayByFrameTime();
	}
}

void Application::Update()
{
	// Update All GameObjects
	
}

void Application::Draw()
{
	// Render all GameObjects before before clearing the buffer


	Renderer::PresentBuffer();
}

void Application::Clean()
{
	delete engineSubSystems;
	engineSubSystems = nullptr;
}