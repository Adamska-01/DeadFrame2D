#include "Background.h"
#include <Constants/ScreenConstants.h>
#include <SDL.h>
#include <SubSystems/TextureManager.h>


Background::Background(std::string texturePath, Vector2 start, bool canScroll)
	: canScroll(canScroll)
{
	//set camera and image rects
	srcCamera.x = start.x;
	srcCamera.y = start.y;
	srcCamera.w = ScreenConstants::SCREENWIDTH;
	srcCamera.h = ScreenConstants::SCREENHEIGHT;
	
	scrollingSpeed = 50;
	increment = 0;

	backgroundTexture = TextureManager::GetInstance()->LoadTexture(texturePath);

	SDL_QueryTexture(backgroundTexture, NULL, NULL, &srcImage.w, &srcImage.h);
}

void Background::Update(float dt)
{
	if (!canScroll)
		return;
	
	increment += scrollingSpeed * dt;
	if (increment >= 1.0f)
	{
		srcCamera.x += (int)increment;
		increment = 0;
	}

	if (srcCamera.x > srcImage.w - srcCamera.w)
	{
		srcCamera.x = 0;
	}
}

void Background::Draw()
{
	TextureManager::GetInstance()->DrawPortion(backgroundTexture, srcCamera, srcImage, Vector2(0.3f, 0.3f));
}