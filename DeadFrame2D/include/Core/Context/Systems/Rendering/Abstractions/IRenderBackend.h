#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/Rendering/BlendMode.h"
#include "Data/Systems/Rendering/RenderFlip.h"
#include "Data/Systems/Rendering/Vertex2D.h"
#include "DF2D_API.h"
#include <cstdint>
#include <optional>
#include <span>


namespace DF2D::Core
{
	/**
	 * @brief The drawing surface every rendering backend implements.
	 */
	class DF2D_API IRenderBackend
	{
	public:
		virtual ~IRenderBackend() = default;


		/**
		 * @brief Draws a single pixel.
		 */
		virtual void DrawPixel(
			const Vector2F& pixelPos,
			Color color = Constants::CommonColors::WHITE,
			Data::BlendMode blendMode = Data::BlendMode::ALPHA) = 0;

		/**
		 * @brief Draws a one-pixel-wide line between two points.
		 */
		virtual void DrawLine(
			const Vector2F& p1,
			const Vector2F& p2, Color color = Constants::CommonColors::WHITE,
			Data::BlendMode blendMode = Data::BlendMode::ALPHA) = 0;

		/**
		 * @brief Draws a rectangle, rotated about its own centre.
		 */
		virtual void DrawRect(
			RectF rect,
			float angleDegrees,
			Color color = Constants::CommonColors::WHITE,
			bool filled = false,
			Data::BlendMode blendMode = Data::BlendMode::ALPHA) = 0;

		/**
		 * @brief Draws a circle approximated by a fixed number of segments.
		 */
		virtual void DrawCircle(
			const Vector2F& center,
			float radius, Color color,
			bool filled, Data::BlendMode blendMode = Data::BlendMode::ALPHA) = 0;

		/**
		 * @brief Draws a texture, optionally cropped, scaled, rotated and tinted.
		 *
		 * Does nothing if the texture is unknown to the backend.
		 */
		virtual void DrawTexture(
			Data::TextureID textureID,
			const std::optional<RectI>& srcRect = std::nullopt,
			const std::optional<RectF>& dstRect = std::nullopt,
			const std::optional<Vector2F>& rotationOrigin = std::nullopt,
			float angle = 0.0f,
			Data::RenderFlip flip = Data::RenderFlip::NONE,
			Color colorMod = Constants::CommonColors::WHITE,
			Data::BlendMode blendMode = Data::BlendMode::ALPHA) = 0;

		/**
		 * @brief Draws an indexed triangle mesh.
		 *
		 * Index values are relative to the start of the supplied vertex span, so a caller replaying a
		 * GeometryDrawList hands over one command's slice of each pool rather than the whole buffer.
		 */
		virtual void DrawGeometry(
			Data::TextureID textureID,
			std::span<const Data::Vertex2D> vertices,
			std::span<const uint32_t> indices,
			const Vector2F& translation = Vector2F::Zero,
			Data::BlendMode blendMode = Data::BlendMode::ALPHA) = 0;

		/**
		 * @brief Restricts drawing to a rectangle of the current render target.
		 *
		 * This is renderer state, not per-draw state: it stays applied until changed, so a caller that
		 * sets it is responsible for clearing it again once its own draws are done.
		 */
		virtual void SetClipRect(const std::optional<RectI>& clipRect) = 0;


		/**
		 * @brief Directs subsequent draws at an off-screen target.
		 */
		virtual void SetRenderTarget(Data::TextureID renderTarget) = 0;

		/**
		 * @brief Fills the bound render target with the current display color.
		 *
		 * The color is written rather than blended, so clearing to a transparent color really does
		 * reset the target's alpha.
		 */
		virtual void ClearCurrentRenderTarget() = 0;

		/** @brief Publishes everything drawn since the last present to the screen. */
		virtual void Present() = 0;

		/**
		 * @brief Creates an off-screen target that can be both drawn into and drawn as a texture.
		 *
		 * @return The new target, or 0 on failure.
		 */
		virtual Data::TextureID CreateRenderTarget(int w, int h) = 0;

		/** @brief Destroys a texture or render target and releases its ID. */
		virtual void DestroyTexture(Data::TextureID id) = 0;


		/** @brief Returns the color used by primitive draws and by ClearCurrentRenderTarget. */
		virtual Color GetDisplayColor() = 0;

		/** @brief Returns the logical resolution every coordinate passed to this interface is in. */
		virtual Core::Vector2I GetResolutionTarget() = 0;

		/**
		 * @brief Restricts drawing to a sub-rectangle of the target and moves the coordinate origin.
		 *
		 * Unlike SetClipRect, this also shifts where (0, 0) lands.
		 */
		virtual void SetViewport(RectI viewPort) = 0;

		/** @brief Sets the color used by primitive draws and by ClearCurrentRenderTarget. */
		virtual void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;

		/**
		 * @brief Sets the logical resolution the backend scales to the window.
		 *
		 * Implementations are expected to announce the change so targets sized against the old
		 * resolution (such as each camera's) can be rebuilt.
		 */
		virtual void SetResolutionTarget(Core::Vector2I targetResolution) = 0;
	};
}