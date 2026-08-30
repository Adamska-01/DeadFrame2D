#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "DF2D_API.h"
#include <cstdint>
#include <span>
#include <string>


namespace DF2D::Core
{
	/**
	 * @brief Creates and releases the textures the renderer draws with.
	 *
	 * Textures are identified by an opaque Data::TextureID, where 0 means invalid;
	 */
	class DF2D_API ITextureBackend
	{
	public:
		virtual ~ITextureBackend() = default;


		/** @brief Loads an image file. Returns the new texture, or 0 on failure. */
		virtual Data::TextureID LoadFromFile(const std::string& filename) = 0;

		/**
		 * @brief Creates a texture from raw 32-bit RGBA pixels, for images that have no file behind
		 * them such as the glyph atlases the UI backend builds at runtime.
		 *
		 * @param rgba: Tightly packed RGBA8 pixels, size.x * size.y * 4 bytes.
		 * @return The new texture, or 0 on failure.
		 */
		virtual Data::TextureID CreateFromPixels(std::span<const uint8_t> rgba, Vector2I size) = 0;

		/** @brief Destroys a texture. Unknown or already-released ids are ignored. */
		virtual void UnloadTexture(Data::TextureID id) = 0;

		/** @brief Returns the texture's size in pixels, or zero if the id is unknown. */
		virtual Vector2I GetTextureSize(Data::TextureID id) = 0;
	};
}