#pragma once
#include <map>
#include <SDL_ttf.h>
#include "Generic/Fonts.h"


struct SDL_Texture;


class UIManager
{
private:
	UIManager();


	static UIManager* Instance;


	std::map<Fonts, TTF_Font*> fontList;


public:
	inline static UIManager* GetInstance() { return Instance = (Instance != nullptr) ? Instance : new UIManager(); }


	bool LoadFont(Fonts id, std::string textPath, int fontsize = 20);

	SDL_Texture* LoadText(Fonts font, std::string text, SDL_Color color, unsigned int numRows);
	
	void DrawText(SDL_Texture* texture, SDL_Rect dest, Vector2 scale);

	void Clean();

	const std::map<Fonts, TTF_Font*>& GetFontList();
};