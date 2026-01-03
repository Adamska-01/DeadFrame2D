#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "Core/SubSystems/Systems/Rendering/RenderSystem.h"
#include "DF2D_API.h"
#include "Engine/Entity/ComponentHandle.h"
#include <cstdint>
#include <Models/Rendering/RendererConfig.h>
#include <SDL.h>


namespace DeadFrame2D::Engine
{
	class Camera;
}


namespace DeadFrame2D::Core
{
	class DF2D_API Renderer : protected RenderSystem, public ISubSystem
	{
		friend class SubSystemManager;


	private:
		static SDL_Renderer* renderer;


		Renderer(SDL_Window* window, const Shared::Models::RendererConfig& config);

		~Renderer() override;

		Renderer(const Renderer&) = delete;

		Renderer(Renderer&&) = delete;

		Renderer& operator=(const Renderer&) = delete;

		Renderer& operator=(Renderer&&) = delete;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		static void DrawPixel(const Vector2F& pixelPos, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE);
		
		static void DrawLine(const Vector2F& p1, const Vector2F& p2, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE);

		static void DrawRect(SDL_FRect rect, float angleDegrees, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE, bool filled = false);

		static void DrawCircle(const Vector2F& center, float radius, SDL_Color color, bool filled);

		static void DrawTexture(
			SDL_Texture* texture,
			const SDL_Rect* srcRect = NULL,
			const SDL_FRect* dstRect = NULL,
			const SDL_FPoint* rotationOrigin = NULL,
			float angle = 0.0f,
			SDL_RendererFlip flip = SDL_FLIP_NONE,
			SDL_Color colorMod = DeadFrame2D::Constants::CommonColors::WHITE);


		static void DrawFromTask(DeadFrame2D::Data::RenderTask& renderTask, DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera = {}, bool requiresSreenSpaceConversion = true);


		static void ClearAndPresentBuffer();

		static SDL_Renderer* GetRenderer();

		static SDL_Color GetDisplayColor();

		static Vector2I GetResolutionTarget();

		static void SetViewport(const SDL_Rect& viewPort);

		static void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		static void SetResolutionTarget(Vector2I targetResolution);
	};
}