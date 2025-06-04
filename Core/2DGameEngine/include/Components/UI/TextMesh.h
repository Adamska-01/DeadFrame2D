#pragma once
#include "Components/UI/Abstractions/UIComponent.h"
#include "Data/UI/FontStyle.h"
#include "Math/Vector2.h"
#include <memory>
#include <string>


struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;
struct TextMeshComponentModel;
class Transform;


class TextMesh : public UIComponent
{
private:
	Transform* transform;

	Vector2F initialObjectScale;

	std::shared_ptr<SDL_Texture> textTexture;
	
	std::shared_ptr<TTF_Font> font;

	std::string text;
	
	SDL_Color color;
	
	Vector2F textRectSize;

	bool centered;

	unsigned int fontSize;


public:
	TextMesh(const TextMeshComponentModel& textMeshConfiguration);
	
	virtual ~TextMesh() override = default;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetFontSize(unsigned int newFontSize);
	
	void SetTextColor(SDL_Color newColor);
	
	void SetFontStyle(FontStyle newFontStyle);

	void SetText(std::string newText);

	void SetIsCentered(bool isCentered);

	std::string GetText();

	SDL_Rect GetTextBoundingBox();
};