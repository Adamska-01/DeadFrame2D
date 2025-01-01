#pragma once
#include <string>
#include <Math/Vector2.h>
#include <SubSystems/UIManager.h>


struct SDL_Texture;


class UIText
{
private:
	SDL_Texture* texture;
	
	std::shared_ptr<TTF_Font> font;

	std::string Text;
	
	SDL_Color color;
	
	SDL_Rect textRect;


public:
	UIText(std::string fontPath, std::string text);
	
	~UIText() = default;


	void Draw(Vector2 scale, bool center = false);

	void ChangeColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	
	void SetPos(int x, int y);
	
	void SetWidthAndHeight(int w, int h);
	
	SDL_Texture* UpdateText(std::string newText, unsigned int numRows);

	SDL_Rect GetTextureDim();
};