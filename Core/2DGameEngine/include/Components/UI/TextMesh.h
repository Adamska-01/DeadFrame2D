#pragma once
#include "Components/GameComponent.h"
#include "Data/UI/FontStyle.h"
#include <memory>
#include <string>


struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;
struct TextMeshComponentModel;
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
	TextMesh(const TextMeshComponentModel& textMeshConfiguration);
	
	virtual ~TextMesh() override = default;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetFontSize(unsigned int newFontSize);
	
	void SetTextColor(SDL_Color newColor);
	
	void SetFontStyle(FontStyle newFontStyle);

	void SetText(std::string newText, unsigned int numRows);

	SDL_Rect GetTextBoundingBox();
};