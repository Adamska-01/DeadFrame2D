#include <Components/Generic/SpriteAnimationProperties.h>


void SpriteAnimationProperties::Reset()
{
	loop = true;
	animSpeed = 0.0f;
	sourceRowNumber = 0;
	columnCount = 0;
	rowCount = 0;
	flip = SDL_FLIP_NONE;
}