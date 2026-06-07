#include "Constants/Paths/ResourcePaths.h"
#include "Core/Context/SystemInitializer.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/DeadFrameRuntime.h"
#include "Core/Services/ServiceInitializer.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "Models/Other/SplashScreenConfig.h"
#include "Models/SystemConfig.h"
#include <algorithm>


namespace DF2D::Core
{
	using namespace DF2D::Constants;
	using namespace DF2D::Models;
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Utilities;


	DeadFrameRuntime::DeadFrameRuntime()
	{
		auto systemConfig = SystemConfig::LoadFromFiles();

		systemInitializer = std::make_unique<SystemInitializer>(systemConfig);
		serviceInitializer = std::make_unique<ServiceInitializer>(systemConfig);

		auto coreCtx = systemInitializer->GetCoreContext();
		auto serviceCtx = serviceInitializer->GetServiceContext();

		serviceCtx.sceneManager->SetContexts(coreCtx, serviceCtx);
	}

	DeadFrameRuntime::~DeadFrameRuntime()
	{
		// Delete subsystems last
		serviceInitializer.reset();
		systemInitializer.reset();
	}

	std::optional<int> DeadFrameRuntime::RenderSplashScreen()
	{
		auto coreCtx = systemInitializer->GetCoreContext();
		auto splashTexture = coreCtx.textureManager->LoadTexture(Paths::Files::SPLASH_SCREEN);
		auto renderTargetSize = Renderer::GetResolutionTarget();

		auto size = coreCtx.textureManager->GetTextureSize(splashTexture);

		auto splashScreenConfig = JsonSerializer::DeserializeFromFile<SplashScreenConfig>(Paths::Files::SPLASH_SCREEN_CONFIGURATION);

		constexpr uint8_t MaxAlpha = 255;

		auto fadeInDuration = splashScreenConfig.fadeInDurationSeconds;
		auto holdDuration = splashScreenConfig.holdVisibleDurationSeconds;
		auto fadeOutDuration = splashScreenConfig.fadeOutDurationSeconds;
		auto totalDuration = fadeInDuration + holdDuration + fadeOutDuration;

		auto elapsedTime = 0.0f;

		auto renderTask = RenderTask();
		renderTask.renderPhase = RenderPhase::SCREEN_SPACE_OVERLAY_UI;

		auto renderData = SpriteRenderData
		{
			.texture = coreCtx.textureManager->GetRawTexture(splashTexture),
			.destRect = SDL_FRect
			{
				renderTargetSize.x * 0.5f - size.x * 0.2f,
				renderTargetSize.y * 0.5f - size.y * 0.2f,
				size.x * 0.4f,
				size.y * 0.4f
			}
		};

		auto serviceCtx = serviceInitializer->GetServiceContext();
		auto frameTimer = serviceCtx.frameTimer;
		auto eventManager = serviceCtx.eventManager;

		while (elapsedTime < totalDuration)
		{
			frameTimer->StartClock();

			if (const auto ecode = eventManager->ProcessEvents())
				return *ecode;

			elapsedTime += frameTimer->DeltaTime();

			auto alpha = static_cast<float>(MaxAlpha);

			if (elapsedTime < fadeInDuration)
			{
				alpha *= elapsedTime / fadeInDuration;
			}
			else if (elapsedTime > totalDuration - fadeOutDuration)
			{
				alpha *= 1.0f - ((elapsedTime - (totalDuration - fadeOutDuration)) / fadeOutDuration);
			}

			alpha = std::clamp(alpha, 0.0f, 255.0f);
			renderData.colorMod.a = static_cast<uint8_t>(alpha);

			renderTask.renderData = renderData;

			RenderSystem::Submit(renderTask);

			Renderer::ClearAndPresentBuffer();

			frameTimer->EndClock();
			frameTimer->DelayByFrameTime();
		}

		return std::nullopt;
	}

	std::optional<int> DeadFrameRuntime::Run()
	{
		if (const auto& splashCode = RenderSplashScreen())
			return *splashCode;

		auto serviceCtx = serviceInitializer->GetServiceContext();
		auto frameTimer = serviceCtx.frameTimer;
		auto eventManager = serviceCtx.eventManager;
		auto sceneManager = serviceCtx.sceneManager;

		while (true)
		{
			frameTimer->StartClock();

			auto deltaTime = frameTimer->DeltaTime();

			systemInitializer->BeginFrame();

			// Only returns in case of QUIT event
			if (const auto& ecode = eventManager->ProcessEvents())
				return *ecode;

			systemInitializer->PreUpdate(deltaTime);

			sceneManager->UpdateScene(deltaTime);

			systemInitializer->EndUpdate(deltaTime);

			sceneManager->LateUpdateScene(deltaTime);

			sceneManager->DrawScene();

			systemInitializer->EndDraw();

			Renderer::ClearAndPresentBuffer();

			sceneManager->LoadNewSceneIfAvailable();

			// FPS and delay
			frameTimer->EndClock();
			frameTimer->DelayByFrameTime();
		}
	}
}