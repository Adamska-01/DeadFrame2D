#pragma once
#include "Components/GameComponent.h"
#include "Data/UI/FontStyle.h"
#include <memory>
#include <string>


struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;
class Transform;


class TextMesh : public GameComponent
{
private:
	Transform* transform;

	std::shared_ptr<SDL_Texture> textTexture;
	
	std::shared_ptr<TTF_Font> font;

	std::string text;
	
	SDL_Color color;
	
	Vector2F textRectSize;

	unsigned int fontSize;

	unsigned int linesNumber;


public:
	TextMesh(std::string_view fontSource, std::string text, unsigned int fontSize, unsigned int linesNumber);
	
	virtual ~TextMesh() override = default;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetFontSize(unsigned int newFontSize);
	
	void SetTextColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	
	void SetFontStyle(FontStyle newFontStyle);

	void SetText(std::string newText, unsigned int numRows);

	SDL_Rect GetTextBoundingBox();
};