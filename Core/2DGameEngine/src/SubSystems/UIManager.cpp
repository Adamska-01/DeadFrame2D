#include <Debugging/Debug.h>
#include <Math/Vector2.h>
#include <SubSystems/Renderer.h>
#include <SubSystems/UIManager.h>
#include <SubSystems/TextureManager.h>


UIManager* UIManager::Instance = nullptr;


UIManager::UIManager()
{
	if (TTF_Init() < 0)
	{
		std::cerr << "Failed to initialize SDL_ttf: " << SDL_GetError() << std::endl;
	}
};

bool UIManager::LoadFont(Fonts id, std::string textPath, int fontsize)
{
	fontList[id] = TTF_OpenFont(textPath.c_str(), fontsize);

#if _DEBUG
		DBG_ASSERT_MSG(fontList[id], "Failed to load the text: % s\n", TTF_GetError());
#endif

	return true;
}

SDL_Texture* UIManager::LoadText(Fonts font, std::string text, SDL_Color color, unsigned int numRows)
{
	if (text.empty())
		return nullptr;

	auto totalWidth = 0, totalHeight = 0;
	if (TTF_SizeText(fontList[font], text.c_str(), &totalWidth, &totalHeight) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to calculate text size: %s", TTF_GetError());
		return nullptr;
	}

	Uint32 wrapSize = totalWidth / numRows;

	wrapSize = wrapSize > 0 ? wrapSize : 1;

	auto textSurface = TTF_RenderText_Blended_Wrapped(fontList[font], text.c_str(), color, wrapSize);

#if _DEBUG
	DBG_ASSERT_MSG(textSurface, "Failed to load the texture: % s\n", TTF_GetError());
#endif

	auto textTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetRenderer(), textSurface);

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

	SDL_RenderCopy(Renderer::GetInstance()->GetRenderer(), texture, NULL, &destRect);
}

void UIManager::Clean()
{
	for (auto it = fontList.begin(); it != fontList.end(); it++)
	{
		TTF_CloseFont(it->second);
	}

	fontList.clear();

#if _DEBUG
	DBG_ASSERT_MSG_EMPTY(fontList.size(), "DEBUG_MSG: FontList cleaned! \n");
#endif

	TTF_Quit();
}

const std::map<Fonts, TTF_Font*>& UIManager::GetFontList() 
{ 
	return fontList;
}