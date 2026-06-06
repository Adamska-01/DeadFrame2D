#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Factories/Products/Context/Systems/Graphics/SDLTextureBackend.h"
#include <iostream>
#include <SDL_image.h>


namespace DF2D::Internal
{
	using namespace DF2D::Core;


	SDLTextureBackend::SDLTextureBackend()
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
		for (auto& [id, tex] : textures)
		{
			if (tex) SDL_DestroyTexture(tex);
		}

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

		auto texture = SDL_CreateTextureFromSurface(Renderer::GetRenderer(), tempSurface);

		SDL_FreeSurface(tempSurface);

		if (!texture)
		{
			std::cerr << "Failed to create texture from: " << filename << " Error: " << SDL_GetError() << std::endl;

			return 0;
		}

		auto id = nextId++;
		textures[id] = texture;

		return id;
	}

	void SDLTextureBackend::UnloadTexture(Data::TextureID id)
	{
		auto it = textures.find(id);

		if (it != textures.end())
		{
			if (it->second) SDL_DestroyTexture(it->second);
			textures.erase(it);
		}
	}

	Vector2I SDLTextureBackend::GetTextureSize(Data::TextureID id)
	{
		auto it = textures.find(id);

		if (it != textures.end() && it->second)
		{
			int w = 0, h = 0;
			SDL_QueryTexture(it->second, nullptr, nullptr, &w, &h);
			return Vector2I{ w, h };
		}

		return Vector2I::Zero;
	}

	void* SDLTextureBackend::GetNativeHandle(Data::TextureID id)
	{
		auto it = textures.find(id);

		return it != textures.end() ? it->second : nullptr;
	}
}
