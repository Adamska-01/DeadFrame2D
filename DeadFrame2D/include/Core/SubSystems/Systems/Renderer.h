#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include <cstdint>
#include <Models/Rendering/RendererConfig.h>
#include <SDL.h>


namespace DeadFrame2D::Engine
{
	class Camera;
}


namespace DeadFrame2D::Core
{
	class DF2D_API Renderer : public ISubSystem
	{
		friend class SubSystemManager;


	private:
		static SDL_Renderer* renderer;

		static DeadFrame2D::Engine::Camera* activeCamera;


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

		static void DrawRect(SDL_Rect rect, float angleDegrees, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE, bool filled = false);

		static void DrawCircle(const Vector2F& center, float radius, SDL_Color color, bool filled);

		static void DrawTexture(
			SDL_Texture* texture,
			const SDL_Rect* srcRect = NULL,
			const SDL_FRect* dstRect = NULL,
			float angle = 0.0f,
			SDL_FPoint* rotationOrigin = NULL,
			SDL_RendererFlip flip = SDL_FLIP_NONE,
			Uint8 alpha = 255,
			SDL_Color colorMod = DeadFrame2D::Constants::CommonColors::WHITE);


		static void ClearAndPresentBuffer();

		static void BeginCamera(DeadFrame2D::Engine::Camera* camera);

		static void EndCamera();

		static SDL_Renderer* GetRenderer();

		static DeadFrame2D::Engine::Camera* GetActiveCamera();

		static SDL_Color GetDisplayColor();

		static Vector2I GetResolutionTarget();

		static void SetViewport(const SDL_Rect& viewPort);

		static void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		static void SetResolutionTarget(Vector2I targetResolution);
	};
}