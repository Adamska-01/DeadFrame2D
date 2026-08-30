#pragma once
#include "Core/Context/Systems/Graphics/Abstractions/ITextureBackend.h"
#include "Data/TextureRegistry.h"
#include "DF2D_API.h"


struct SDL_Renderer;


namespace DF2D::Internal
{
	class DF2D_API SDLTextureBackend : public Core::ITextureBackend
	{
	private:
		SDL_Renderer* renderer;

		TextureRegistry* registry;


	public:
		SDLTextureBackend(SDL_Renderer* renderer, TextureRegistry* textureRegistry);

		~SDLTextureBackend() override;

		SDLTextureBackend(const SDLTextureBackend&) = delete;

		SDLTextureBackend(SDLTextureBackend&&) = delete;

		SDLTextureBackend& operator=(const SDLTextureBackend&) = delete;

		SDLTextureBackend& operator=(SDLTextureBackend&&) = delete;


		Data::TextureID LoadFromFile(const std::string& filename) override;

		Data::TextureID CreateFromPixels(std::span<const uint8_t> rgba, Core::Vector2I size) override;

		void UnloadTexture(Data::TextureID id) override;

		Core::Vector2I GetTextureSize(Data::TextureID id) override;
	};
}