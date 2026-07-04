#pragma once
#include "Data/Systems/Graphics/TextureID.h"
#include <SDL_render.h>
#include <unordered_map>


namespace DF2D::Internal
{
	struct TextureRegistry
	{
		std::unordered_map<::DF2D::Data::TextureID, SDL_Texture*> textures;

		Data::TextureID nextID = 1;


		Data::TextureID AddTexture(SDL_Texture* tex)
		{
			auto id = nextID++;

			textures[id] = tex;

			return id;
		}

		void RemoveTexture(Data::TextureID id, bool destroy)
		{
			auto it = textures.find(id);
			if (it == textures.end())
				return;

			if (destroy && it->second)
			{
				SDL_DestroyTexture(it->second);
			}

			textures.erase(it);
		}

		SDL_Texture* GetTexture(Data::TextureID id) const
		{
			auto it = textures.find(id);

			return it != textures.end() ? it->second : nullptr;
		}
	};
}