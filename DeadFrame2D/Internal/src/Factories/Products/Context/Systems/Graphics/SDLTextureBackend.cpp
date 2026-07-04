#include "Factories/Products/Context/Systems/Graphics/SDLTextureBackend.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>


namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	SDLTextureBackend::SDLTextureBackend(SDL_Renderer* renderer, TextureRegistry* textureRegistry)
		: renderer(renderer),
		registry(textureRegistry)
	{
		auto initFlags = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

		if ((initFlags & (IMG_INIT_PNG | IMG_INIT_JPG)) != (IMG_INIT_PNG | IMG_INIT_JPG))
		{
			std::cerr << "Failed to initialize SDL_image with PNG/JPG support! Error: " << IMG_GetError() << std::endl;

			return;
		}

		std::cout << "[Info] SDL_image successfully initialized." << std::endl;
	}

	SDLTextureBackend::~SDLTextureBackend()
	{
		IMG_Quit();

		std::cout << "[Info] SDL_image subsystem successfully quit." << std::endl;
	}


	Data::TextureID SDLTextureBackend::LoadFromFile(const std::string& filename)
	{
		auto tempSurface = IMG_Load(filename.c_str());

		if (!tempSurface)
		{
			std::cerr << "Failed to load image: " << filename << " Error: " << IMG_GetError() << std::endl;

			return 0;
		}

		auto texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

		SDL_FreeSurface(tempSurface);

		if (!texture)
		{
			std::cerr << "Failed to create texture from: " << filename << " Error: " << SDL_GetError() << std::endl;

			return 0;
		}

		return registry->AddTexture(texture);
	}

	void SDLTextureBackend::UnloadTexture(Data::TextureID id)
	{
		registry->RemoveTexture(id, true);
	}

	Vector2I SDLTextureBackend::GetTextureSize(Data::TextureID id)
	{
		auto* texture = registry->GetTexture(id);

		if (texture == nullptr)
			return Vector2I::Zero;

		int w = 0, h = 0;

		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

		return Vector2I(w, h);
	}
}