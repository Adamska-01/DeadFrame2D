#pragma once
#include "Components/GameComponent.h"
#include "Data/Animation/SpriteAnimationProperties.h"
#include "Data/Animation/SpriteAnimationState.h"
#include <string>
#include <unordered_map>


class Transform;
class Sprite;


class SpriteAnimator : public GameComponent
{
protected:
	Transform* transform;

	Sprite* sprite;

	std::unordered_map<std::string, SpriteAnimationProperties> animations;

	std::string currentAnimationID;

	SpriteAnimationState animState;


public:
	SpriteAnimator();

	
	virtual void Init() override;

	virtual void Start() override;
	
	virtual void Update(float dt) override;
	
	virtual void Draw() override;


	void AddAnimation(const SpriteAnimationProperties& properties);

	void PlayAnimation(const std::string& name, bool restartIfPlaying = false);
	
	bool IsPlaying(const std::string& name) const;

	void SetFlipState(SDL_RendererFlip flipState);

	float GetAnimationProgressRatio() const;

	const SpriteAnimationProperties* GetCurrentAnimationProperties() const;

	SDL_Rect GetFrameRect() const;
};