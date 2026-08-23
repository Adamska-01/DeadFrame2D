#include "Constants/Rendering/DefaultSortOrders.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Core/Math/Vector2.h"
#include "Engine/ECS/Component/Animation/SpriteAnimator.h"
#include "Engine/ECS/Component/Rendering/SpriteRenderer.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


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
		sprite = Guard::AgainstNullAssignment(gameObject->GetComponent<SpriteRenderer>(), NAME_OF(sprite));
		textureManager = Guard::AgainstNullAssignment(gameObject->CoreContext().textureManager, NAME_OF(textureManager));

		sprite->SetActive(false);

		if (!animations.empty())
		{
			sprite->LoadSprite(animations.at(currentAnimationID).spriteSource);
		}
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
				animState.currentFrame = 0.0f;
			}
			else
			{
				animState.currentFrame = static_cast<float>(props.columnCount - 1);
				animState.finished = true;
			}
		}
	}

	void SpriteAnimator::Draw()
	{
		auto spriteTextureID = sprite->GetTexture();

		if (!spriteTextureID || animations.empty())
			return;

		const auto& props = animations.at(currentAnimationID);

		const auto frameRect = GetFrameRect();
		const auto frameIndex = static_cast<int>(animState.currentFrame);

		const auto position = transform->GetWorldPosition();
		const auto scale = transform->GetWorldScale();

		auto srcRect = RectI
		{
			.x = frameRect.w * frameIndex,
			.y = frameRect.h * props.sourceRowNumber,
			.w = frameRect.w,
			.h = frameRect.h
		};

		auto dstRect = RectF
		{
			.x = std::round(position.x - (frameRect.w * scale.x * 0.5f)),
			.y = std::round(position.y - (frameRect.h * scale.y * 0.5f)),
			.w = frameRect.w * scale.x,
			.h = frameRect.h * scale.y
		};

		renderTask.renderData = SpriteRenderData
		{
			.texture = spriteTextureID,
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

		if (properties.columnCount <= 0 || properties.rowCount <= 0)
			return;

		if (properties.sourceRowNumber < 0 || properties.sourceRowNumber >= properties.rowCount)
			return;

		animations[properties.name] = properties;

		if (animations.size() == 1)
		{
			currentAnimationID = properties.name;

			animState = SpriteAnimationState();

			if (sprite != nullptr)
			{
				sprite->LoadSprite(properties.spriteSource);
			}
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

	void SpriteAnimator::SetFlipState(RenderFlip flipState)
	{
		animState.flipState = flipState;
	}

	float SpriteAnimator::GetAnimationProgressRatio() const
	{
		if (animations.empty())
			return 0.0f;

		if (animState.finished)
			return 1.0f;

		const auto& props = animations.at(currentAnimationID);

		return animState.currentFrame / static_cast<float>(props.columnCount);
	}

	const SpriteAnimationProperties* SpriteAnimator::GetCurrentAnimationProperties() const
	{
		if (animations.find(currentAnimationID) != animations.end())
			return &animations.at(currentAnimationID);

		return nullptr;
	}

	RectI SpriteAnimator::GetFrameRect() const
	{
		auto size = textureManager->GetTextureSize(sprite->GetTexture());

		const auto& props = animations.at(currentAnimationID);

		const int frameWidth = size.x / props.columnCount;
		const int frameHeight = size.y / props.rowCount;

		return RectI
		{
			.x = 0,
			.y = 0,
			.w = frameWidth,
			.h = frameHeight
		};
	}
}