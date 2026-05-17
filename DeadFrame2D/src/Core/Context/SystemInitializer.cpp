#include "Core/Context/SystemInitializer.h"
#include "Core/Context/Systems/Audio/AudioManager.h"
#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Core/Context/Systems/Window/Window.h"
#include "Factories/Concretions/Context/Systems/Audio/AudioBackendFactory.h"
#include "Helpers/Context/CoreContextIterator.h"


namespace DF2D::Core
{
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Models;
	using namespace DF2D::Factories;
	using namespace DF2D::Internal;


	SystemInitializer::SystemInitializer(SystemConfig config)
	{
		auto window = new Window(config.window);

		auto renderer = new Renderer(window->GetWindow(), config.rendering);

		ctx = CoreContext
		{
			.audioManager = new AudioManager(config.audio, AudioBackendFactory().CreateProduct(config.audio)),
			.coroutineScheduler = new CoroutineScheduler(),
			.textureManager = new TextureManager(),
			.input = new Input(),
			.physicsEngine = new PhysicsEngine2D(config.physics),
			.renderer = renderer,
			.uiManager = new UIManager(),
			.window = window
		};
	}

	SystemInitializer::~SystemInitializer()
	{
		CoreContextIterator::ForEach(ctx, [](auto*& system)
		{
			delete system;
			system = nullptr;
		});

		if (SDL_WasInit(SDL_INIT_EVERYTHING))
		{
			SDL_Quit();

			std::cout << "[Info] SDL successfully quit.\n";
		}
	}


	void SystemInitializer::BeginFrame()
	{
		CoreContextIterator::ForEach(ctx, [](auto* system)
			{
				system->BeginFrame();
			});
	}

	void SystemInitializer::PreUpdate(float deltaTime)
	{
		CoreContextIterator::ForEach(ctx, [&](auto* system)
			{
				system->PreUpdate(deltaTime);
			});
	}

	void SystemInitializer::EndUpdate(float deltaTime)
	{
		CoreContextIterator::ForEach(ctx, [&](auto* system)
			{
				system->EndUpdate(deltaTime);
			});
	}

	void SystemInitializer::EndDraw()
	{
		CoreContextIterator::ForEach(ctx, [](auto* system)
			{
				system->EndDraw();
			});
	}

	CoreContext SystemInitializer::GetCoreContext() const
	{
		return ctx;
	}
}