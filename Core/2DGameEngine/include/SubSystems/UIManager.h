#pragma once
#include "Tools/PairHash.h"
#include <memory>
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>


struct SDL_Texture;


class UIManager
{
	friend class SubSystems;


private:
	UIManager();

	~UIManager();

	UIManager(const UIManager&) = delete;
	
	UIManager(UIManager&&) = delete;
	

	UIManager& operator=(const UIManager&) = delete;

	UIManager& operator=(UIManager&&) = delete;


	static std::unordered_map<std::pair<std::string, int>, std::shared_ptr<TTF_Font>, PairHash> fontCache;


public:
	static std::shared_ptr<TTF_Font> LoadFont(std::string_view textSource, int fontsize);

	static std::shared_ptr<SDL_Texture> LoadText(std::shared_ptr<TTF_Font> font, std::string text, SDL_Color color, unsigned int linesNumber);
};