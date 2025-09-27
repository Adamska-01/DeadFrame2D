#include "Core/Engine.h"
#include "Core/SubSystems/Systems/Renderer.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Engine/Components/Rendering/Camera.h"
#include <Constants/ResourcePaths.h>
#include <Models/Other/SplashScreenConfig.h>
#include <Models/SystemConfig.h>


namespace DeadFrame2D::Core
{
	using namespace Shared::Constants;
	using namespace Shared::Models;

	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Utilities;


	Engine::Engine()
	{
		auto systemConfig = SystemConfig::LoadFromFiles();

		engineSubSystems = std::make_unique<SubSystems>();
		engineSubSystems->InitializeSubSystems(systemConfig);

		sceneManager = std::make_unique<SceneManager>();

		frameTimer.SetTargetFramerate(systemConfig.rendering.targetFramerate);
	}

	std::optional<int> Engine::RenderSplashScreen()
	{
		auto splashTexture = TextureManager::LoadTexture(Paths::Files::SPLASH_SCREEN);
		auto renderer = Renderer::GetRenderer();
		auto renderTargetSize = Renderer::GetResolutionTarget();

		auto width = 0, height = 0;
		SDL_QueryTexture(splashTexture.get(), nullptr, nullptr, &width, &height);

		auto destRect = SDL_Rect
		{
			static_cast<int>(renderTargetSize.x * 0.5f - width * 0.2f),
			static_cast<int>(renderTargetSize.y * 0.5f - height * 0.2f),
			static_cast<int>(width * 0.4f),
			static_cast<int>(height * 0.4f)
		};

		auto splashScreenConfig = Shared::Tools::DeserializeFromFile<SplashScreenConfig>(Paths::Files::SPLASH_SCREEN_CONFIGURATION);

		constexpr uint8_t MaxAlpha = 255;
		auto fadeInDuration = splashScreenConfig.fadeInDurationSeconds;
		auto holdDuration = splashScreenConfig.holdVisibleDurationSeconds;
		auto fadeOutDuration = splashScreenConfig.fadeOutDurationSeconds;
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
				alpha = static_cast<uint8_t>(alpha * t);
			}
			else if (elapsedTime > totalDuration - fadeOutDuration)
			{
				auto t = 1.0f - ((elapsedTime - (totalDuration - fadeOutDuration)) / fadeOutDuration);
				alpha = static_cast<uint8_t>(alpha * t);
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

			for (const auto& camera : Camera::cameras)
			{
				if (!camera->IsActive())
					continue;

				TextureManager::currentCamera = camera;

				Renderer::SetViewport(camera->GetViewBox());

				sceneManager->DrawScene();

				engineSubSystems->EndDraw();

				TextureManager::currentCamera = nullptr;
			}

			Renderer::PresentBuffer();

			sceneManager->LoadNewSceneIfAvailable();

			//FPS and delay
			frameTimer.EndClock();
			frameTimer.DelayByFrameTime();
		}
	}
}