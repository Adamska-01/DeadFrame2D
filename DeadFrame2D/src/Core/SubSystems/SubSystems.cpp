#include "Core/SubSystems/SubSystems.h"
#include "Core/SubSystems/Systems/AudioManager.h"
#include "Core/SubSystems/Systems/CoroutineScheduler.h"
#include "Core/SubSystems/Systems/Input/Input.h"
#include "Core/SubSystems/Systems/PhysicsEngine2D.h"
#include "Core/SubSystems/Systems/Renderer.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Core/SubSystems/Systems/UIManager.h"
#include "Core/SubSystems/Systems/Window.h"


namespace DeadFrame2D::Core
{
	using namespace Shared::Constants;
	using namespace Shared::Models;


	SubSystems::SubSystems()
	{
		subSystems.fill(nullptr);
	}

	SubSystems::~SubSystems()
	{
		for (const auto& subSystem : subSystems)
		{
			delete subSystem;
		}
	}

	void SubSystems::InitializeSubSystems(SystemConfig config)
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

	void SubSystems::Update(float deltaTime)
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->Update(deltaTime);
		}
	}

	void SubSystems::BeginFrame()
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->BeginFrame();
		}
	}

	void SubSystems::EndUpdate()
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->EndUpdate();
		}
	}

	void SubSystems::EndDraw()
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->EndDraw();
		}
	}
}