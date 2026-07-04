#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/Rendering/RenderFlip.h"
#include "DF2D_API.h"
#include <cstdint>
#include <optional>


namespace DF2D::Core
{
	class DF2D_API IRenderBackend
	{
	public:
		virtual ~IRenderBackend() = default;


		virtual void DrawPixel(const Vector2F& pixelPos, Color color = Constants::CommonColors::WHITE) = 0;

		virtual void DrawLine(const Vector2F& p1, const Vector2F& p2, Color color = Constants::CommonColors::WHITE) = 0;

		virtual void DrawRect(RectF rect, float angleDegrees, Color color = Constants::CommonColors::WHITE, bool filled = false) = 0;

		virtual void DrawCircle(const Vector2F& center, float radius, Color color, bool filled) = 0;

		virtual void DrawTexture(
			Data::TextureID textureID,
			const std::optional<RectI>& srcRect = std::nullopt,
			const std::optional<RectF>& dstRect = std::nullopt,
			const std::optional<Vector2F>& rotationOrigin = std::nullopt,
			float angle = 0.0f,
			Data::RenderFlip flip = Data::RenderFlip::NONE,
			Color colorMod = Constants::CommonColors::WHITE) = 0;

		virtual void SetRenderTarget(Data::TextureID renderTarget) = 0;

		virtual void ClearCurrentRenderTarget() = 0;

		virtual void Present() = 0;

		virtual Data::TextureID CreateRenderTarget(int w, int h) = 0;

		virtual void DestroyTexture(Data::TextureID id) = 0;

		virtual Color GetDisplayColor() = 0;

		virtual Core::Vector2I GetResolutionTarget() = 0;

		virtual void SetViewport(RectI viewPort) = 0;

		virtual void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

		virtual void SetResolutionTarget(Core::Vector2I targetResolution) = 0;
	};
}