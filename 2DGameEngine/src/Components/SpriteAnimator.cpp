#include "Components/Rendering/Sprite.h"
#include "Components/SpriteAnimator.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "Math/Vector2.h"
#include "SubSystems/TextureManager.h"


SpriteAnimator::SpriteAnimator()
{
	started = false;
	spriteFrame = 0.0f;
	animationProperties = SpriteAnimationProperties();
	spriteTexture = nullptr;
	transform = nullptr;
	sprite = nullptr;
}

void SpriteAnimator::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();
	sprite = OwningObject.lock()->GetComponent<Sprite>();

	if (sprite == nullptr)
		return;

	sprite->SetActive(false);
}

void SpriteAnimator::Start()
{

}

void SpriteAnimator::Update(float dt)
{
	spriteTexture = sprite->GetTexture();

	if (!spriteTexture) 
		return;

	// Increment sprite frame
	spriteFrame += animationProperties.animSpeed * dt;

	if (spriteFrame >= animationProperties.columnCount)
	{
		// Reset started for non-repeating animations
		if (!animationProperties.loop)
		{
			started = false;
		}
	}
	else if (!animationProperties.loop && !started)
	{
		spriteFrame = 0;
		started = true;
	}
}

void SpriteAnimator::Draw()
{
	auto dstRect = GetFrameRect();
	auto currentPosition = transform->GetWorldPosition();
	auto currentScale = transform->GetWorldScale();

	dstRect.x = round(currentPosition.x - ((float)dstRect.w * currentScale.x) / 2);
	dstRect.y = round(currentPosition.y - ((float)dstRect.h * currentScale.y) / 2);

	auto srcRect = SDL_Rect
	{
		dstRect.w * (int)spriteFrame,
		dstRect.h * animationProperties.sourceRowNumber,
		dstRect.w,
		dstRect.h
	};

	dstRect.w *= currentScale.x;
	dstRect.h *= currentScale.y;

	TextureManager::DrawTexture(spriteTexture, &srcRect, &dstRect, transform->GetWorldRotation(), NULL, animationProperties.flip);
}

float SpriteAnimator::GetAnimationProgressRatio()
{
	return spriteFrame / static_cast<float>(animationProperties.columnCount);
}

void SpriteAnimator::SetAnimationProperties(const SpriteAnimationProperties& newProperties)
{
	animationProperties = newProperties;

	spriteFrame = 0;
	started = false;
}

SDL_Rect SpriteAnimator::GetFrameRect()
{
	SDL_Rect srcRect;
	SDL_QueryTexture(sprite->GetTexture().get(), NULL, NULL, &srcRect.w, &srcRect.h);

	//Get the size of a single frame in a sprite sheet
	srcRect.w = static_cast<int>(round((float)srcRect.w / animationProperties.columnCount));
	srcRect.h = static_cast<int>(round((float)srcRect.h / animationProperties.rowCount));

	return srcRect;
}

const SpriteAnimationProperties& SpriteAnimator::GetProp()
{
	return animationProperties;
}