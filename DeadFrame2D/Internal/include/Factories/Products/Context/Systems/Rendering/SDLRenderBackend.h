#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/TextureRegistry.h"
#include "DF2D_API.h"
#include "Models/Rendering/RendererConfig.h"


struct SDL_Renderer;

struct SDL_Window;


namespace DF2D::Internal
{
	class DF2D_API SDLRenderBackend : public Core::IRenderBackend
	{
	private:
		SDL_Renderer* renderer = nullptr;

		TextureRegistry textureRegistry;

		Core::Vector2I resolutionTarget;


	public:
		SDLRenderBackend(SDL_Window* window, const Models::RendererConfig& config);

		~SDLRenderBackend() override;

		SDLRenderBackend(const SDLRenderBackend&) = delete;

		SDLRenderBackend(SDLRenderBackend&&) = delete;

		SDLRenderBackend& operator=(const SDLRenderBackend&) = delete;

		SDLRenderBackend& operator=(SDLRenderBackend&&) = delete;


		void DrawPixel(const Core::Vector2F& pixelPos, Core::Color color = Constants::CommonColors::WHITE) override;

		void DrawLine(const Core::Vector2F& p1, const Core::Vector2F& p2, Core::Color color = Constants::CommonColors::WHITE) override;

		void DrawRect(Core::RectF rect, float angleDegrees, Core::Color color = Constants::CommonColors::WHITE, bool filled = false) override;

		void DrawCircle(const Core::Vector2F& center, float radius, Core::Color color, bool filled) override;

		void DrawTexture(
			Data::TextureID textureID,
			const std::optional<Core::RectI>& srcRect = std::nullopt,
			const std::optional<Core::RectF>& dstRect = std::nullopt,
			const std::optional<Core::Vector2F>& rotationOrigin = std::nullopt,
			float angle = 0.0f,
			Data::RenderFlip flip = Data::RenderFlip::NONE,
			Core::Color colorMod = Constants::CommonColors::WHITE) override;

		void SetRenderTarget(Data::TextureID renderTarget) override;

		void ClearCurrentRenderTarget() override;

		void Present() override;

		Data::TextureID CreateRenderTarget(int w, int h) override;

		void DestroyTexture(Data::TextureID id) override;

		Core::Color GetDisplayColor() override;

		Core::Vector2I GetResolutionTarget() override;

		void SetViewport(Core::RectI viewPort) override;

		void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

		void SetResolutionTarget(Core::Vector2I targetResolution) override;


		TextureRegistry* GetTextureRegistry()
		{
			return &textureRegistry;
		}

		SDL_Renderer* GetSDLRenderer()
		{
			return renderer;
		}
	};
}