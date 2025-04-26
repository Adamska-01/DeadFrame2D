#include "Debugging/Debug.h"
#include "Math/Vector2.h"
#include "SubSystems/Renderer.h"
#include "SubSystems/UIManager.h"


std::unordered_map<std::pair<std::string, int>, std::shared_ptr<TTF_Font>, PairHash> UIManager::fontCache = {};


UIManager::UIManager()
{
	if (TTF_Init() < 0)
	{
		std::cerr << "Failed to initialize SDL_TTF: " << SDL_GetError() << std::endl;

		return;
	}

	std::cout << "[Info] SDL_TTF successfully initialized." << std::endl;
}

UIManager::~UIManager()
{
	fontCache.clear();

	TTF_Quit();

	std::cout << "[Info] SDL_TTF subsystem successfully quit." << std::endl;
}

std::shared_ptr<TTF_Font> UIManager::LoadFont(std::string_view textSource, int fontsize)
{
	auto filenameString = std::string(textSource);

	// Check if the font is already cached
	auto key = std::make_pair(filenameString, fontsize);
	auto it = fontCache.find(key);
	
	if (it != fontCache.end()) 
		return it->second;

	// If the font is not in the cache, load it from the file
	auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(filenameString.c_str(), fontsize), TTF_CloseFont);

	// Check if the font loaded successfully
#if _DEBUG
	DBG_ASSERT_MSG(font, "Failed to load the font: %s\n", TTF_GetError());
#endif

	// Store the font in the cache (shared_ptr automatically manages the memory)
	fontCache[key] = font;

	return font;
}

std::shared_ptr<SDL_Texture> UIManager::LoadText(std::shared_ptr<TTF_Font> font, std::string text, SDL_Color color, unsigned int linesNumber)
{
	if (text.empty())
		return nullptr;

	auto totalWidth = 0, totalHeight = 0;
	if (TTF_SizeText(font.get(), text.c_str(), &totalWidth, &totalHeight) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to calculate text size: %s", TTF_GetError());
		return nullptr;
	}

	Uint32 wrapSize = totalWidth / linesNumber;

	wrapSize = wrapSize > 0 ? wrapSize : 1;

	auto textSurface = TTF_RenderText_Blended_Wrapped(font.get(), text.c_str(), color, wrapSize);

#if _DEBUG
	DBG_ASSERT_MSG(textSurface, "Failed to load the texture: % s\n", TTF_GetError());
#endif

	auto textTexture = SDL_CreateTextureFromSurface(Renderer::GetRenderer(), textSurface);

#if _DEBUG
	DBG_ASSERT_MSG(textTexture, "Failed to create the texture: % s\n", TTF_GetError());
#endif

	// Deleting temp surface
	SDL_FreeSurface(textSurface);
	textSurface = nullptr;
	
	return std::shared_ptr<SDL_Texture>(textTexture, SDL_DestroyTexture);
}