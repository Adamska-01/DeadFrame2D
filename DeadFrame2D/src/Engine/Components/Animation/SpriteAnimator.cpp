#include "Constants/Rendering/DefaultSortOrders.h"
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Rendering/RenderSystem.h"
#include "Engine/Components/Animation/SpriteAnimator.h"
#include "Engine/Components/Rendering/Sprite.h"
#include "Engine/Components/Transform.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	SpriteAnimator::SpriteAnimator()
		: currentAnimationID(""),
		animState(SpriteAnimationState())
	{
		animations.clear();

		renderTask.renderPhase = RenderPhase::WORLD;
		renderTask.sortOrder = DefaultSortOrders::SPRITE_RENDERER;
	}

	void SpriteAnimator::Init()
	{
		auto gameObject = GetGameObject();

		transform = Guard::AgainstNullAssignment(gameObject->GetTransform(), NAME_OF(transform));
		sprite = Guard::AgainstNullAssignment(gameObject->GetComponent<Sprite>(), NAME_OF(sprite));

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

		const auto frameRect = GetFrameRect();
		const auto frameIndex = static_cast<int>(animState.currentFrame);

		const auto position = transform->GetWorldPosition();
		const auto scale = transform->GetWorldScale();

		auto srcRect = SDL_Rect
		{
			.x = frameRect.w * frameIndex,
			.y = frameRect.h * props.sourceRowNumber,
			.w = frameRect.w,
			.h = frameRect.h
		};

		auto dstRect = SDL_FRect
		{
			.x = std::round(position.x - (frameRect.w * scale.x * 0.5f)),
			.y = std::round(position.y - (frameRect.h * scale.y * 0.5f)),
			.w = frameRect.w * scale.x,
			.h = frameRect.h * scale.y
		};

		renderTask.renderData = SpriteRenderData
		{
			.texture = spriteTexture.get(),
			.srcRect = srcRect,
			.destRect = dstRect,
			.flip = animState.flipState,
			.rotation = transform->GetWorldRotation()
		};

		RenderSystem::Submit(renderTask);
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
		auto textureW = 0;
		auto textureH = 0;

		SDL_QueryTexture(
			sprite->GetTexture().get(),
			nullptr,
			nullptr,
			&textureW,
			&textureH);

		//Get the size of a single frame in a sprite sheet
		const auto& props = animations.at(currentAnimationID);

		const int frameWidth = textureW / props.columnCount;
		const int frameHeight = textureH / props.rowCount;

		return SDL_Rect
		{
			.x = 0,
			.y = 0,
			.w = frameWidth,
			.h = frameHeight
		};
	}
}