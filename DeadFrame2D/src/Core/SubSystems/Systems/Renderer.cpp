#include "Core/Debugging/Debug.h"
#include "Core/SubSystems/Systems/Renderer.h"
#include "Core/SubSystems/Systems/Window.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Renderer/RenderTargetSizeChangedEvent.h"
#include <SDL.h>


namespace DeadFrame2D::Core
{
	using namespace Shared::Models;

	using namespace DeadFrame2D::Engine;


	SDL_Renderer* Renderer::renderer = nullptr;

	Camera* Renderer::activeCamera = nullptr;


	Renderer::Renderer(SDL_Window* window, const RendererConfig& config)
	{
	#if _DEBUG
		DBG_ASSERT_MSG(window, "Window initialisation failed: %s\n", SDL_GetError());
	#endif

		//startup
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "Failed to Initialize SDL_VIDEO: " << SDL_GetError() << std::endl;
		}

		//create the renderer
		renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED);

	#if _DEBUG
		DBG_ASSERT_MSG(renderer, "Renderer initialisation failed: %s\n", SDL_GetError());
	#endif

		SetResolutionTarget({ config.width, config.height });

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		std::cout << "[Info] SDL_VIDEO successfully initialized." << std::endl;
	}

	Renderer::~Renderer()
	{
		if (renderer == nullptr) 
		{
			std::cout << "[Info] Renderer is already null, no need to destroy." << std::endl;

			return;
		}

		SDL_DestroyRenderer(renderer);

		SDL_Quit();

		std::cout << "[Info] Renderer successfully destroyed and SDL_VIDEO successfully quit." << std::endl;
	}

	void Renderer::BeginFrame()
	{

	}

	void Renderer::PreUpdate(float deltaTime)
	{

	}

	void Renderer::EndUpdate(float deltaTime)
	{

	}

	void Renderer::EndDraw()
	{

	}

	void Renderer::ClearAndPresentBuffer()
	{
		// Backbuffer
		SDL_SetRenderTarget(renderer, NULL);

		SetDisplayColor(0, 0, 0, 255);

		SDL_RenderClear(renderer);

		for (auto camera : Camera::GetCameras())
		{
			if (!camera->IsActive())
				continue;

			auto viewport = camera->GetNormalizedViewBox();

			SDL_RenderCopy(
				renderer,
				camera->GetRenderTarget(),
				nullptr,
				&viewport);
		}

		SDL_RenderPresent(renderer);
	}

	void Renderer::BeginCamera(Camera* camera)
	{
		activeCamera = camera;

		SDL_SetRenderTarget(renderer, camera->GetRenderTarget());
	}

	void Renderer::EndCamera()
	{
		activeCamera = nullptr;
	}

	SDL_Renderer* Renderer::GetRenderer()
	{
		return renderer;
	}

	Camera* Renderer::GetActiveCamera()
	{
		return activeCamera;
	}

	SDL_Color Renderer::GetDisplayColor()
	{
		Uint8 r, g, b, a;

		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

		return SDL_Color(r, g, b, a);
	}

	Vector2I Renderer::GetResolutionTarget()
	{
		auto width = 0;
		auto height = 0;

		if (renderer != nullptr)
		{
			SDL_RenderGetLogicalSize(renderer, &width, &height);
		}

		return Vector2I(width, height);
	}

	void Renderer::SetViewport(const SDL_Rect& viewPort)
	{
		SDL_RenderSetViewport(renderer, &viewPort);
	}

	void Renderer::SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		if (renderer == nullptr)
			return;

		//set background color
		auto result = SDL_SetRenderDrawColor(
			renderer,
			r,
			g,
			b,
			a);

	#if _DEBUG
		DBG_ASSERT_MSG(!result, "Background color initialisation failed: %s\n", SDL_GetError());
	#endif
	}

	void Renderer::SetResolutionTarget(Vector2I targetResolution)
	{
		auto width = targetResolution.x;
		auto height = targetResolution.y;

		if (width <= 0 || height <= 0)
			return;

		SDL_RenderSetLogicalSize(renderer, width, height);

		EventDispatcher::SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I(width, height)));
	}
}