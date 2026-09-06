#include "Constants/Paths/ResourcePaths.h"
#include "Core/Context/SystemInitializer.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/UI/UIManager.h"
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

		serviceInitializer = std::make_unique<ServiceInitializer>(systemConfig);

		auto serviceCtx = serviceInitializer->GetServiceContext();

		systemInitializer = std::make_unique<SystemInitializer>(systemConfig, serviceCtx);

		auto coreCtx = systemInitializer->GetCoreContext();

		// Order is irrelevant: sinks cannot consume, so both see every event. The UI needs them to do its
		// hit testing and focus tracking, the input devices to mirror the hardware.
		serviceCtx.eventManager->AddSink(coreCtx.uiManager);
		serviceCtx.eventManager->AddSink(coreCtx.input);

		serviceCtx.sceneManager->SetContexts(coreCtx, serviceCtx);
	}

	DeadFrameRuntime::~DeadFrameRuntime()
	{
		// Services go first so scenes (and their components) are torn down while the core systems they use are still alive.
		// Core systems only read service pointers during a frame, never while being destroyed.
		serviceInitializer.reset();
		systemInitializer.reset();
	}

	std::optional<int> DeadFrameRuntime::RenderSplashScreen()
	{
		auto coreCtx = systemInitializer->GetCoreContext();
		auto splashTexture = coreCtx.textureManager->LoadTexture(Paths::Files::SPLASH_SCREEN);
		auto renderTargetSize = coreCtx.renderer->GetResolutionTarget();

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

		auto renderData = SpriteRenderData();
		renderData.texture = splashTexture;
		renderData.destRect = Core::RectF(
			renderTargetSize.x * 0.5f - size.x * 0.2f,
			renderTargetSize.y * 0.5f - size.y * 0.2f,
			size.x * 0.4f,
			size.y * 0.4f);

		auto serviceCtx = serviceInitializer->GetServiceContext();
		auto frameTimer = serviceCtx.frameTimer;
		IFrameCycle* frameCycle = static_cast<FrameTimer*>(frameTimer);
		auto eventManager = serviceCtx.eventManager;

		while (elapsedTime < totalDuration)
		{
			frameCycle->BeginFrame();

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

			coreCtx.renderer->ClearAndPresentBuffer();

			frameCycle->EndFrame();
		}

		return std::nullopt;
	}

	std::optional<int> DeadFrameRuntime::Run()
	{
		if (const auto& splashCode = RenderSplashScreen())
			return *splashCode;

		auto coreCtx = systemInitializer->GetCoreContext();
		auto serviceCtx = serviceInitializer->GetServiceContext();
		auto frameTimer = serviceCtx.frameTimer;
		IFrameCycle* frameCycle = static_cast<FrameTimer*>(frameTimer);
		auto eventManager = serviceCtx.eventManager;
		auto sceneManager = serviceCtx.sceneManager;

		while (true)
		{
			frameCycle->BeginFrame();

			auto deltaTime = frameTimer->DeltaTime();

			systemInitializer->BeginFrame();

			if (const auto& ecode = eventManager->ProcessEvents())
				return *ecode;

			systemInitializer->PreUpdate(deltaTime);

			sceneManager->UpdateScene(deltaTime);

			systemInitializer->EndUpdate(deltaTime);

			sceneManager->LateUpdateScene(deltaTime);

			sceneManager->DrawScene();

			systemInitializer->EndDraw();

			coreCtx.renderer->ClearAndPresentBuffer();

			sceneManager->LoadNewSceneIfAvailable();

			frameCycle->EndFrame();
		}
	}
}