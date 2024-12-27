#include <Components/Generic/SpriteAnimationProperties.h>


SpriteAnimationProperties::SpriteAnimationProperties()
{
	loop = false;
	animSpeed = 0.0f;
	sourceRowNumber = 0;
	columnCount = 0;
	rowCount = 0;
	flip = SDL_RendererFlip::SDL_FLIP_NONE;
}

SpriteAnimationProperties::SpriteAnimationProperties(
	bool loop,
	float animSpeed,
	int sourceRowNumber,
	int columnCount,
	int rowCount,
	SDL_RendererFlip flip)
		: loop(loop),
		animSpeed(animSpeed),
		sourceRowNumber(sourceRowNumber),
		columnCount(columnCount),
		rowCount(rowCount),
		flip(flip)
{
}

void SpriteAnimationProperties::Reset()
{
	loop = true;
	animSpeed = 0.0f;
	sourceRowNumber = 0;
	columnCount = 0;
	rowCount = 0;
	flip = SDL_FLIP_NONE;
}