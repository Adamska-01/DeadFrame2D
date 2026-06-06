#pragma once
#include "Core/Context/Systems/Graphics/Abstractions/ITextureBackend.h"
#include "DF2D_API.h"
#include <unordered_map>


struct SDL_Texture;


namespace DF2D::Internal
{
	class DF2D_API SDLTextureBackend final : public Core::ITextureBackend
	{
	private:
		std::unordered_map<Data::TextureID, SDL_Texture*> textures;

		Data::TextureID nextId = 1;


	public:
		SDLTextureBackend();

		~SDLTextureBackend() override;


		Data::TextureID LoadFromFile(const std::string& filename) override;

		void UnloadTexture(Data::TextureID id) override;

		Core::Vector2I GetTextureSize(Data::TextureID id) override;

		void* GetNativeHandle(Data::TextureID id) override;
	};
}
