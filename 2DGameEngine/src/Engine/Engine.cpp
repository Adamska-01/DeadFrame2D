#include "Engine/Engine.h"
#include "SubSystems/Renderer.h"
#include <Constants/ResourcePaths.h>
#include <SubSystems/TextureManager.h>
#include <Tools/JsonSerializer.h>


Engine::Engine()
{
	engineConfig = JsonSerializer::DeserializeFromFile<EngineConfig>(Constants::ResourcePaths::Files::ENGINE_CONFIGURATION);

	engineSubSystems = std::make_unique<SubSystems>();
	engineSubSystems->InitializeSubSystems(engineConfig);

	sceneManager = std::make_unique<SceneManager>();

	frameTimer.SetTargetFramerate(engineConfig.rendering.targetFramerate);
}

std::optional<int> Engine::RenderSplashScreen()
{
	auto splashTexture = TextureManager::LoadTexture(Constants::ResourcePaths::Files::SPLASH_SCREEN);
	auto renderer = Renderer::GetRenderer();
	auto renderTargetSize = Renderer::GetResolutionTarget();

	auto width = 0, height = 0;
	SDL_QueryTexture(splashTexture.get(), nullptr, nullptr, &width, &height);

	auto destRect = SDL_Rect
	{
		static_cast<int>(renderTargetSize.x * 0.5f - width * 0.25f),
		static_cast<int>(renderTargetSize.y * 0.5f - height * 0.25f),
		static_cast<int>(width * 0.5f),
		static_cast<int>(height * 0.5f)
	};

	constexpr uint8_t MaxAlpha = 255;
	auto fadeInDuration = engineConfig.splashScreen.fadeInDurationSeconds;
	auto holdDuration = engineConfig.splashScreen.holdVisibleDurationSeconds;
	auto fadeOutDuration = engineConfig.splashScreen.fadeOutDurationSeconds;
	auto totalDuration = fadeInDuration + holdDuration + fadeOutDuration;

	auto elapsedTime = 0.0f;
	while (elapsedTime < totalDuration)
	{
		frameTimer.StartClock();

		if (const auto ecode = eventManager.ProcessEvents())
			return *ecode;

		auto deltaTime = frameTimer.DeltaTime();

		elapsedTime += deltaTime;

		auto alpha = MaxAlpha;

		if (elapsedTime < fadeInDuration)
		{
			auto t = elapsedTime / fadeInDuration;
			alpha *= t;
		}
		else if (elapsedTime > totalDuration - fadeOutDuration)
		{
			auto t = 1.0f - ((elapsedTime - (totalDuration - fadeOutDuration)) / fadeOutDuration);
			alpha *= t;
		}

		Renderer::ClearBuffer();
		TextureManager::DrawTexture(splashTexture, nullptr, &destRect, 0.0f, nullptr, SDL_RendererFlip::SDL_FLIP_NONE, alpha);
		Renderer::PresentBuffer();

		frameTimer.EndClock();
		frameTimer.DelayByFrameTime();
	}

	return std::nullopt;
}

std::optional<int> Engine::Run()
{
	if (const auto splashCode = RenderSplashScreen())
		return *splashCode;

	while (true)
	{
		frameTimer.StartClock();

		auto deltaTime = frameTimer.DeltaTime();

		engineSubSystems->BeginFrame();

		// Looks for messages and return optional if QUIT
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