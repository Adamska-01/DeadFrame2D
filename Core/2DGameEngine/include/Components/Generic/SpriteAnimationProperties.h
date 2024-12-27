#pragma once
#include <SDL.h>


struct SpriteAnimationProperties
{
	bool loop;

	float animSpeed;

	int sourceRowNumber;

	int columnCount;

	int rowCount;

	SDL_RendererFlip flip;


	SpriteAnimationProperties();

	SpriteAnimationProperties(
		bool loop,
		float animSpeed,
		int sourceRowNumber,
		int columnCount,
		int rowCount,
		SDL_RendererFlip flip);


	void Reset();
};