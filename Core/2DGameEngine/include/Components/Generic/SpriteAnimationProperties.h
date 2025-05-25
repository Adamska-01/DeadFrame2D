#pragma once
#include <SDL.h>


struct SpriteAnimationProperties
{
	bool loop = false;

	float animSpeed = 1.0f;

	int sourceRowNumber = 0;

	int columnCount = 0;

	int rowCount = 0;

	SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE;


	void Reset();
};