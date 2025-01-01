#include <Debugging/Debug.h>
#include <Math/Vector2.h>
#include <SubSystems/Renderer.h>
#include <SubSystems/TextureManager.h>
#include <SubSystems/UIManager.h>


std::unordered_map<std::pair<std::string, int>, std::shared_ptr<TTF_Font>, PairHash> UIManager::fontCache = {};


std::shared_ptr<TTF_Font> UIManager::LoadFont(std::string textPath, int fontsize)
{
	// Check if the font is already cached
	auto key = std::make_pair(textPath, fontsize);
	auto it = fontCache.find(key);
	
	if (it != fontCache.end()) 
		return it->second;

	// If the font is not in the cache, load it from the file
	auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(textPath.c_str(), fontsize), TTF_CloseFont);

	// Check if the font loaded successfully
#if _DEBUG
		DBG_ASSERT_MSG(font, "Failed to load the font: %s\n", TTF_GetError());
#endif

	// Store the font in the cache (shared_ptr automatically manages the memory)
	fontCache[key] = font;

	return font;
}

SDL_Texture* UIManager::LoadText(TTF_Font* font, std::string text, SDL_Color color, unsigned int numRows)
{
	if (text.empty())
		return nullptr;

	auto totalWidth = 0, totalHeight = 0;
	if (TTF_SizeText(font, text.c_str(), &totalWidth, &totalHeight) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to calculate text size: %s", TTF_GetError());
		return nullptr;
	}

	Uint32 wrapSize = totalWidth / numRows;

	wrapSize = wrapSize > 0 ? wrapSize : 1;

	auto textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, wrapSize);

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
	
	return textTexture;
}

void UIManager::DrawText(SDL_Texture* texture, SDL_Rect dest, Vector2 scale)
{
	SDL_Rect destRect = { dest.x, dest.y, dest.w * scale.x, dest.h * scale.y };

	SDL_RenderCopy(Renderer::GetRenderer(), texture, NULL, &destRect);
}

void UIManager::DrawDebugTextBox(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect textRect)
{
	SDL_SetRenderDrawColor(Renderer::GetRenderer(), r, g, b, a);
	SDL_RenderDrawRect(Renderer::GetRenderer(), &textRect);
}