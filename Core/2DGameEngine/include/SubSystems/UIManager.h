#pragma once
#include "Utility/PairHash.h"
#include <map>
#include <SDL_ttf.h>


struct SDL_Texture;


class UIManager
{
private:
	UIManager() = default;

	UIManager(const UIManager&) = delete;
	
	UIManager(UIManager&&) = delete;
	

	UIManager& operator=(const UIManager&) = delete;

	UIManager& operator=(UIManager&&) = delete;


	static std::unordered_map<std::pair<std::string, int>, std::shared_ptr<TTF_Font>, PairHash> fontCache;


public:
	static std::shared_ptr<TTF_Font> LoadFont(std::string textPath, int fontsize);

	static SDL_Texture* LoadText(TTF_Font* font, std::string text, SDL_Color color, unsigned int numRows);
	
	static void DrawText(SDL_Texture* texture, SDL_Rect dest, Vector2 scale);

	static void DrawDebugTextBox(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect textRect);
};