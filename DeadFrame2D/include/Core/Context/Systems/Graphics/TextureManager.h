#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Graphics/Abstractions/ITextureBackend.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "DF2D_API.h"
#include <cstdint>
#include <span>
#include <string>
#include <unordered_map>


namespace DF2D::Core
{
	class DF2D_API TextureManager : public ICoreSystem
	{
	private:
		std::unordered_map<std::string, Data::TextureID> filenameToID;

		std::unordered_map<Data::TextureID, Vector2I> textureSizes;

		std::unique_ptr<ITextureBackend> backend;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		TextureManager(std::unique_ptr<ITextureBackend> backend);

		~TextureManager() override;

		TextureManager(const TextureManager&) = delete;

		TextureManager(TextureManager&&) = delete;


		TextureManager& operator=(const TextureManager&) = delete;

		TextureManager& operator=(TextureManager&&) = delete;


		Data::TextureID LoadTexture(std::string_view filename);

		/**
		 * @brief Creates an unnamed texture from raw 32-bit RGBA pixels.
		 *
		 * Generated textures have no file path, so they are not entered into the path cache and the
		 * caller owns the returned ID: it must be released with UnloadTexture.
		 *
		 * @param rgba: Tightly packed RGBA8 pixels, size.x * size.y * 4 bytes.
		 * @return The new texture, or 0 on failure.
		 */
		Data::TextureID CreateTexture(std::span<const uint8_t> rgba, Vector2I size);

		/** @brief Releases a single texture. Cached file textures are also dropped from the cache. */
		void UnloadTexture(Data::TextureID id);

		Vector2I GetTextureSize(Data::TextureID id);

		void ClearCache();
	};
}