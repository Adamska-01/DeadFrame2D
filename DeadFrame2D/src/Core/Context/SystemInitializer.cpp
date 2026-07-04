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
#include "Factories/Concretions/Context/Systems/Graphics/GraphicsBackendFactory.h"
#include "Helpers/Context/CoreContextIterator.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Models;
	using namespace DF2D::Factories;
	using namespace DF2D::Internal;


	SystemInitializer::SystemInitializer(SystemConfig config)
	{
		auto graphicsBackends = GraphicsBackendFactory().CreateProduct(config.window, config.rendering);

		ctx = CoreContext
		{
			.audioManager = new AudioManager(config.audio, AudioBackendFactory().CreateProduct(config.audio)),
			.coroutineScheduler = new CoroutineScheduler(),
			.textureManager = new TextureManager(std::move(graphicsBackends.textureBackend)),
			.input = new Input(),
			.physicsEngine = new PhysicsEngine2D(config.physics),
			.renderer = new Renderer(std::move(graphicsBackends.renderBackend)),
			.uiManager = new UIManager(std::move(graphicsBackends.textBackend)),
			.window = new Window(std::move(graphicsBackends.windowBackend))
		};
	}

	SystemInitializer::~SystemInitializer()
	{
		CoreContextIterator::ForEach(ctx, [](auto*& system)
		{
			delete system;
			system = nullptr;
		});
	}


	void SystemInitializer::BeginFrame()
	{
		CoreContextIterator::ForEach(
			ctx,
			[](ICoreSystem* system)
			{
				system->BeginFrame();
			});
	}

	void SystemInitializer::PreUpdate(float deltaTime)
	{
		CoreContextIterator::ForEach(
			ctx,
			[&](ICoreSystem* system)
			{
				system->PreUpdate(deltaTime);
			});
	}

	void SystemInitializer::EndUpdate(float deltaTime)
	{
		CoreContextIterator::ForEach(
			ctx,
			[&](ICoreSystem* system)
			{
				system->EndUpdate(deltaTime);
			});
	}

	void SystemInitializer::EndDraw()
	{
		CoreContextIterator::ForEach(
			ctx,
			[](ICoreSystem* system)
			{
				system->EndDraw();
			});
	}

	Data::CoreContext SystemInitializer::GetCoreContext() const
	{
		return ctx;
	}
}