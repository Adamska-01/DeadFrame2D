#include "Engine/Engine.h"
#include "SubSystems/Renderer.h"
#include <Constants/SharedResourcePaths.h>
#include <Tools/JsonSerializer.h>
#include <SubSystems/TextureManager.h>


Engine::Engine()
{
	engineConfig = JsonSerializer::DeserializeFromFile<EngineConfig>(SharedResourcePaths::Configurations::ENGINE_CONFIGURATION_PATH);

	engineSubSystems = std::make_unique<SubSystems>();
	engineSubSystems->InitializeSubSystems(engineConfig);

	sceneManager = std::make_unique<SceneManager>();

	frameTimer.SetTargetFramerate(engineConfig.rendering.targetFramerate);
}

std::optional<int> Engine::Run()
{
	{
		auto splashScreenTexture = TextureManager::LoadTexture(engineConfig.splashScreen.imageSourcePath);
		auto renderer = Renderer::GetRenderer();
		auto renderTargetSize = Renderer::GetResolutionTarget();

		int width = 0, height = 0;
		SDL_QueryTexture(splashScreenTexture.get(), nullptr, nullptr, &width, &height);

		SDL_Rect destRect{
			static_cast<int>(renderTargetSize.x * 0.5f - (width / 2)),
			static_cast<int>(renderTargetSize.y * 0.5f - (height / 2)),
			width,
			height
		};

		constexpr float fadeDuration = 1.0f; // seconds
		float elapsedTime = 0.0f;
		float totalDuration = static_cast<float>(engineConfig.splashScreen.durationSeconds);

		while (elapsedTime < totalDuration)
		{
			frameTimer.StartClock();
			
			auto deltaTime = frameTimer.DeltaTime();

			elapsedTime += deltaTime;

			Uint8 alpha = 255;

			if (elapsedTime < fadeDuration)
			{
				// Fade in
				alpha = static_cast<Uint8>((elapsedTime / fadeDuration) * 255);
			}
			else if (elapsedTime > totalDuration - fadeDuration)
			{
				// Fade out
				auto fadeOutElapsed = elapsedTime - (totalDuration - fadeDuration);
				alpha = static_cast<Uint8>(255 * (1.0f - fadeOutElapsed / fadeDuration));
			}

			Renderer::ClearBuffer();
			TextureManager::DrawTexture(splashScreenTexture, &destRect, NULL, 0.0f, NULL, SDL_RendererFlip::SDL_FLIP_NONE, alpha);
			Renderer::PresentBuffer();

			frameTimer.EndClock();
		}
	}

	while (true)
	{
		frameTimer.StartClock();

		auto deltaTime = frameTimer.DeltaTime();

		engineSubSystems->BeginFrame();

		//Looks for messages and return optional if QUIT
		if (const auto ecode = eventManager.ProcessEvents())
			return *ecode;

		engineSubSystems->Update(deltaTime);
		
		sceneManager->UpdateScene(deltaTime);

		engineSubSystems->EndUpdate();

		sceneManager->LateUpdateScene(deltaTime);

		Renderer::ClearBuffer();

		sceneManager->DrawScene();

		engineSubSystems->EndDraw();

		Renderer::PresentBuffer();

		sceneManager->LoadNewSceneIfAvailable();

		//FPS and delay
		frameTimer.EndClock();
		frameTimer.DelayByFrameTime();
	}
}