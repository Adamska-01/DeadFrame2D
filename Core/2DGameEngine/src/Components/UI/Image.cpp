#include "Components/Transform.h"
#include "Components/UI/Image.h"
#include "Constants/CommonColors.h"
#include <SubSystems/TextureManager.h>


Image::Image()
	: color(CommonColors::WHITE)
{
}

void Image::Start()
{

}

void Image::Update(float deltaTime)
{

}

void Image::Draw()
{
	auto currentPosition = transform->GetWorldPosition();
	auto worldRotation = transform->GetWorldRotation();
	auto scaledSize = GetWidgetSize();
	auto anchorVector = GetAnchorFromPreset(anchor);

	auto destRect = SDL_Rect
	{
		static_cast<int>(currentPosition.x - ((scaledSize.x) * anchorVector.x)),
		static_cast<int>(currentPosition.y - ((scaledSize.y) * anchorVector.y)),
		static_cast<int>(scaledSize.x),
		static_cast<int>(scaledSize.y)
	};

	if (sourceImage != nullptr)
	{
		TextureManager::DrawTexture(
			sourceImage,
			NULL,
			&destRect,
			worldRotation,
			NULL,
			SDL_RendererFlip::SDL_FLIP_NONE, 
			255,
			color);
	}
	else
	{
		TextureManager::DrawRect(destRect, worldRotation, color, true);
	}
}

void Image::LoadSprite(std::string_view texturePath)
{
	sourceImage = TextureManager::LoadTexture(texturePath);

	auto width = 0, height = 0;
	SDL_QueryTexture(sourceImage.get(), NULL, NULL, &width, &height);

	widgetSize.x = static_cast<float>(width);
	widgetSize.y = static_cast<float>(height);
}

void Image::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}