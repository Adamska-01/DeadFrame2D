#pragma once
#include <SubSystems/TextureManager.h>


class Background
{
private:
	SDL_Rect srcCamera;

	SDL_Rect srcImage;
	
	SDL_Texture* backgroundTexture;

	bool canScroll;
	
	float scrollingSpeed;
	
	float increment;


public:
	Background(std::string texturePath, Vector2 start, bool canScroll = false);

	~Background() = default;


	void Update(float dt);

	void Draw();
};