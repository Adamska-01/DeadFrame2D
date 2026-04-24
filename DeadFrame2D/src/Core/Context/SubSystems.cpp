#include "Core/Context/SubSystemManager.h"
#include "Core/Context/Systems/AudioManager.h"
#include "Core/Context/Systems/CoroutineScheduler.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/TextureManager.h"
#include "Core/Context/Systems/UIManager.h"
#include "Core/Context/Systems/Window.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Models;


	SubSystemManager::SubSystemManager()
	{
		subSystems.fill(nullptr);
	}

	SubSystemManager::~SubSystemManager()
	{
		// Delete in reverse order to respect dependencies
		for (int i = static_cast<int>(subSystems.size()) - 1; i >= 0; --i)
		{
			if (subSystems[i] == nullptr)
				continue;

			delete subSystems[i];
		}

		if (SDL_WasInit(SDL_INIT_EVERYTHING))
		{
			SDL_Quit();

			std::cout << "[Info] SDL successfully quit.\n";
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

	void SubSystemManager::BeginFrame()
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->BeginFrame();
		}
	}

	void SubSystemManager::PreUpdate(float deltaTime)
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->PreUpdate(deltaTime);
		}
	}

	void SubSystemManager::EndUpdate(float deltaTime)
	{
		for (const auto& subSystem : subSystems)
		{
			subSystem->EndUpdate(deltaTime);
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