#include "Core/SubSystems/SubSystemManager.h"
#include "Core/SubSystems/Systems/AudioManager.h"
#include "Core/SubSystems/Systems/CoroutineScheduler.h"
#include "Core/SubSystems/Systems/Input/Input.h"
#include "Core/SubSystems/Systems/Physics/PhysicsEngine2D.h"
#include "Core/SubSystems/Systems/Renderer.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Core/SubSystems/Systems/UIManager.h"
#include "Core/SubSystems/Systems/Window.h"


namespace DeadFrame2D::Core
{
	using namespace Shared::Constants;
	using namespace Shared::Models;


	SubSystemManager::SubSystemManager()
	{
		subSystems.fill(nullptr);
	}

	SubSystemManager::~SubSystemManager()
	{
		for (const auto& subSystem : subSystems)
		{
			delete subSystem;
		}
	}

	void SubSystemManager::InitializeSubSystems(SystemConfig config)
	{
		auto window = new Window(config.window);

		subSystems[0] = window;

		subSystems[1] = new Renderer(window->GetWindow(), config.rendering);

		subSystems[2] = new Input();

		subSystems[3] = new TextureManager();

		subSystems[4] = new UIManager();

		subSystems[5] = new AudioManager(config.audio);

		subSystems[6] = new PhysicsEngine2D(config.physics);

		subSystems[7] = new CoroutineScheduler();
	}

	void SubSystemManager::Update(float deltaTime)
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->Update(deltaTime);
		}
	}

	void SubSystemManager::BeginFrame()
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->BeginFrame();
		}
	}

	void SubSystemManager::EndUpdate()
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->EndUpdate();
		}
	}

	void SubSystemManager::EndDraw()
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->EndDraw();
		}
	}
}