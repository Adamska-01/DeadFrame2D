#pragma once
#include "Components/UI/Abstractions/UIComponent.h"
#include <memory>
#include <SDL.h>
#include <string_view>


class Image : public UIComponent
{
private:
	std::shared_ptr<SDL_Texture> sourceImage;

	SDL_Color color;


public:
	Image();

	virtual ~Image() override = default;


	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void LoadSprite(std::string_view texturePath);

	void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};