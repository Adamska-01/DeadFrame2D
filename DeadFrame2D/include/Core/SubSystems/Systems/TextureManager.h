#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Circle.h"
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include <memory>
#include <SDL_image.h>
#include <string>
#include <unordered_map>


namespace DeadFrame2D::Engine
{
	class Camera;
}


namespace DeadFrame2D::Core
{
	class DF2D_API TextureManager : public ISubSystem
	{
		friend class SubSystemManager;
		friend class DeadFrameRuntime;


	private:
		static std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textureCache;

		static DeadFrame2D::Engine::Camera* currentCamera;


		TextureManager();

		virtual ~TextureManager() override;

		TextureManager(const TextureManager&) = delete;

		TextureManager(TextureManager&&) = delete;


		TextureManager& operator=(const TextureManager&) = delete;

		TextureManager& operator=(TextureManager&&) = delete;


		virtual void BeginFrame() override;

		virtual void PreUpdate(float deltaTime) override;

		virtual void EndUpdate(float deltaTime) override;

		virtual void EndDraw() override;


		static void DrawLine(
			const Vector2F& p1, 
			const Vector2F& p2, 
			SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE, 
			DeadFrame2D::Engine::Camera* camera = nullptr);

		static void DrawRect(
			SDL_Rect rect, 
			float angleDegrees, 
			SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE, 
			bool filled = false, 
			DeadFrame2D::Engine::Camera* camera = nullptr);

		static void DrawCircle(
			Circle circle, 
			SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE, 
			bool filled = false, 
			DeadFrame2D::Engine::Camera* camera = nullptr);

		static void DrawTexture(
			std::shared_ptr<SDL_Texture> texture,
			const SDL_Rect* srcRect = NULL,
			const SDL_Rect* dstRect = NULL,
			float angle = 0.0f,
			SDL_Point* rotationOrigin = NULL,
			SDL_RendererFlip flip = SDL_FLIP_NONE,
			Uint8 alpha = 255,
			SDL_Color colorMod = DeadFrame2D::Constants::CommonColors::WHITE,
			DeadFrame2D::Engine::Camera* camera = nullptr);

		static void DrawPixel(
			const Vector2F& p, 
			SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE,
			DeadFrame2D::Engine::Camera* camera = nullptr);


	public:
		static std::shared_ptr<SDL_Texture> LoadTexture(std::string_view filename);


		static void DrawLineWorldSpace(const Vector2F& p1, const Vector2F& p2, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE);

		static void DrawLineScreenSpace(const Vector2F& p1, const Vector2F& p2, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE);

		static void DrawRectWorldSpace(SDL_Rect rect, float angleDegrees, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE, bool filled = false);

		static void DrawRectScreenSpace(SDL_Rect rect, float angleDegrees, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE, bool filled = false);
	
		static void DrawCircleWorldSpace(Circle circle, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE, bool filled = false);

		static void DrawCircleScreenSpace(Circle circle, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE, bool filled = false);
	
		static void DrawTextureWorldSpace(
			std::shared_ptr<SDL_Texture> texture,
			const SDL_Rect* srcRect = NULL, 
			const SDL_Rect* dstRect = NULL, 
			float angle = 0.0f, 
			SDL_Point* rotationOrigin = NULL,
			SDL_RendererFlip flip = SDL_FLIP_NONE, 
			Uint8 alpha = 255, 
			SDL_Color colorMod = DeadFrame2D::Constants::CommonColors::WHITE);

		static void DrawTextureScreenSpace(
			std::shared_ptr<SDL_Texture> texture,
			const SDL_Rect* srcRect = NULL,
			const SDL_Rect* dstRect = NULL,
			float angle = 0.0f,
			SDL_Point* rotationOrigin = NULL,
			SDL_RendererFlip flip = SDL_FLIP_NONE,
			Uint8 alpha = 255,
			SDL_Color colorMod = DeadFrame2D::Constants::CommonColors::WHITE);

		static void DrawPixelWorldSpace(const Vector2F& p, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE);

		static void DrawPixelScreenSpace(const Vector2F& p, SDL_Color color = DeadFrame2D::Constants::CommonColors::WHITE);
	};
}