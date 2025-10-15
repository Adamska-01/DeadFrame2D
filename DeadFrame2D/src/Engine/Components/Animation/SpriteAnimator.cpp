#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Engine/Components/Animation/SpriteAnimator.h"
#include "Engine/Components/Rendering/Sprite.h"
#include "Engine/Components/Transform.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;


	SpriteAnimator::SpriteAnimator()
		: currentAnimationID(""),
		animState(SpriteAnimationState())
	{
		animations.clear();
	}

	void SpriteAnimator::Init()
	{
		transform = OwningObject.lock()->GetComponent<Transform>();
		sprite = OwningObject.lock()->GetComponent<Sprite>();

		if (sprite == nullptr)
			return;

		sprite->SetActive(false);
	}

	void SpriteAnimator::Update(float dt)
	{
		auto spriteTexture = sprite->GetTexture();

		if (!spriteTexture || animations.empty())
			return;

		const auto& props = animations.at(currentAnimationID);

		animState.currentFrame += props.animSpeed * dt;

		if (animState.currentFrame >= props.columnCount)
		{
			if (props.loop)
			{
				animState.currentFrame = 0;
			}
			else
			{
				animState.currentFrame = static_cast<float>(props.columnCount - 1);
				animState.started = false;
			}
		}
		else if (!props.loop && !animState.started)
		{
			animState.currentFrame = 0;
			animState.started = true;
		}
	}

	void SpriteAnimator::Draw()
	{
		auto spriteTexture = sprite->GetTexture();

		if (!spriteTexture || animations.empty())
			return;

		const auto& props = animations.at(currentAnimationID);
		auto dstRect = GetFrameRect();
		auto currentPosition = transform->GetWorldPosition();
		auto currentScale = transform->GetWorldScale();

		dstRect.x = static_cast<int>(round(currentPosition.x - (dstRect.w * currentScale.x / 2)));
		dstRect.y = static_cast<int>(round(currentPosition.y - (dstRect.h * currentScale.y / 2)));

		auto srcRect = SDL_Rect
		{
			dstRect.w * static_cast<int>(animState.currentFrame),
			dstRect.h * props.sourceRowNumber,
			dstRect.w,
			dstRect.h
		};

		dstRect.w = static_cast<int>(dstRect.w * currentScale.x);
		dstRect.h = static_cast<int>(dstRect.h * currentScale.y);

		TextureManager::DrawTextureWorldSpace(spriteTexture, &srcRect, &dstRect, transform->GetWorldRotation(), nullptr, animState.flipState);
	}

	void SpriteAnimator::AddAnimation(const SpriteAnimationProperties& properties)
	{
		if (properties.name.empty())
			return;
	
		animations[properties.name] = properties;

		if (animations.size() == 1)
		{
			currentAnimationID = properties.name;
		
			animState = SpriteAnimationState();
		}
	}

	void SpriteAnimator::PlayAnimation(const std::string& name, bool restartIfPlaying)
	{
		if (currentAnimationID == name && !restartIfPlaying)
			return;

		auto it = animations.find(name);
	
		if (it == animations.end())
			return;

		sprite->LoadSprite(it->second.spriteSource);
		currentAnimationID = name;
		animState = SpriteAnimationState{};
	}

	bool SpriteAnimator::IsPlaying(const std::string& name) const
	{
		return currentAnimationID == name;
	}

	void SpriteAnimator::SetFlipState(SDL_RendererFlip flipState)
	{
		animState.flipState = flipState;
	}

	float SpriteAnimator::GetAnimationProgressRatio() const
	{
		if (animations.empty())
			return 0.0f;

		const auto& props = animations.at(currentAnimationID);

		return animState.currentFrame / static_cast<float>(props.columnCount);
	}

	const SpriteAnimationProperties* SpriteAnimator::GetCurrentAnimationProperties() const
	{
		if (animations.find(currentAnimationID) != animations.end())
			return &animations.at(currentAnimationID);

		return nullptr;
	}

	SDL_Rect SpriteAnimator::GetFrameRect() const
	{
		SDL_Rect srcRect;
		SDL_QueryTexture(sprite->GetTexture().get(), NULL, NULL, &srcRect.w, &srcRect.h);

		//Get the size of a single frame in a sprite sheet
		srcRect.w = static_cast<int>(round((float)srcRect.w / animations.at(currentAnimationID).columnCount));
		srcRect.h = static_cast<int>(round((float)srcRect.h / animations.at(currentAnimationID).rowCount));

		return srcRect;
	}
}