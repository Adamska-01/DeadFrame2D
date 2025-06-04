#pragma once
#include "Generic/SpriteAnimationProperties.h"
#include <Components/GameComponent.h>
#include <memory>


class Transform;
class Sprite;


class SpriteAnimator : public GameComponent
{
protected:
	bool started;

	float spriteFrame;

	SpriteAnimationProperties animationProperties;

	std::shared_ptr<SDL_Texture> spriteTexture;

	Transform* transform;

	Sprite* sprite;


public:
	SpriteAnimator();

	
	virtual void Init() override;

	virtual void Start() override;
	
	virtual void Update(float dt) override;
	
	virtual void Draw() override;


	float GetAnimationProgressRatio();

	void SetAnimationProperties(const SpriteAnimationProperties& newProperties);
	
	SDL_Rect GetFrameRect();
	
	const SpriteAnimationProperties& GetProp();
};