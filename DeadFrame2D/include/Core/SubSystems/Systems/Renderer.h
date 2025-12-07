#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include <cstdint>
#include <Models/Rendering/RendererConfig.h>
#include <SDL.h>


namespace DeadFrame2D::Core
{
	class DF2D_API Renderer : public ISubSystem
	{
		friend class SubSystemManager;


	private:
		static SDL_Renderer* renderer;


		Renderer(SDL_Window* window, const Shared::Models::RendererConfig& config);

		virtual ~Renderer() override;

		Renderer(const Renderer&) = delete;

		Renderer(Renderer&&) = delete;

		Renderer& operator=(const Renderer&) = delete;

		Renderer& operator=(Renderer&&) = delete;


		virtual void BeginFrame() override;

		virtual void PreUpdate(float deltaTime) override;

		virtual void EndUpdate(float deltaTime) override;

		virtual void EndDraw() override;


	public:
		static void ClearBuffer();

		static void PresentBuffer();

		static SDL_Renderer* GetRenderer();

		static SDL_Color GetDisplayColor();

		static Vector2I GetResolutionTarget();

		static void SetViewport(const SDL_Rect& viewPort);

		static void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		static void SetResolutionTarget(Vector2I targetResolution);
	};
}