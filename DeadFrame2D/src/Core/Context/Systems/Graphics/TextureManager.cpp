#if DEBUG
#include <cassert>
#endif
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include <memory>
#include <SDL_image.h>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Constants;


	std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> TextureManager::textureCache = {};


	TextureManager::TextureManager()
	{
		auto initFlags = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
		if ((initFlags & (IMG_INIT_PNG | IMG_INIT_JPG)) != (IMG_INIT_PNG | IMG_INIT_JPG))
		{
			std::cerr << "Failed to initialize SDL_image with PNG/JPG support! Error: " << IMG_GetError() << std::endl;

			return;
		}

		std::cout << "[Info] SDL_image successfully initialized." << std::endl;
	}

	TextureManager::~TextureManager()
	{
		textureCache.clear();

		IMG_Quit();

		std::cout << "[Info] SDL_image subsystem successfully quit." << std::endl;
	}

	void TextureManager::BeginFrame()
	{

	}

	void TextureManager::PreUpdate(float deltaTime)
	{

	}

	void TextureManager::EndUpdate(float deltaTime)
	{

	}

	void TextureManager::EndDraw()
	{

	}

	std::shared_ptr<SDL_Texture> TextureManager::LoadTexture(std::string_view filename)
	{
		auto filenameString = std::string(filename);

		auto it = textureCache.find(filenameString);

		if (it != textureCache.end())
		{
			if (auto sharedPtr = it->second.lock())
				return sharedPtr;
		}

		auto tempSurface = IMG_Load(filenameString.c_str());

	#if DEBUG
		assert(tempSurface && SDL_GetError());
	#endif

		auto texture = SDL_CreateTextureFromSurface(Renderer::GetRenderer(), tempSurface);

	#if DEBUG
		assert(texture && SDL_GetError());
	#endif

		SDL_FreeSurface(tempSurface);
		tempSurface = nullptr;

		auto sharedPtr = std::shared_ptr<SDL_Texture>(texture, [filenameString](SDL_Texture* texture)
			{
				SDL_DestroyTexture(texture);

				TextureManager::textureCache.erase(filenameString);
			});

		textureCache[filenameString] = sharedPtr;

		return sharedPtr;
	}
}