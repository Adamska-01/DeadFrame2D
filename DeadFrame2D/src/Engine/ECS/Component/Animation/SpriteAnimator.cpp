#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Math/Vector2.h"
#include "Engine/ECS/Component/Animation/SpriteAnimator.h"
#include "Engine/ECS/Component/Rendering/SpriteRenderer.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"
#include <optional>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	SpriteAnimator::SpriteAnimator()
		: currentAnimationID(""),
		animState(SpriteAnimationState())
	{
		animations.clear();
	}

	SpriteAnimator::~SpriteAnimator()
	{
		if (spriteRenderer == nullptr)
			return;

		spriteRenderer->SetSourceRect(std::nullopt);
		spriteRenderer->SetDrawSize(std::nullopt);
		spriteRenderer->SetFlipState(RenderFlip::NONE);
	}


	void SpriteAnimator::OnNewComponentAddedHandler(const ComponentHandle<GameComponent>& comp)
	{
		if (spriteRenderer != nullptr)
			return;

		auto asRenderer = ComponentHandle<SpriteRenderer>::SafeCast(comp);

		if (asRenderer == nullptr)
			return;

		spriteRenderer = asRenderer;

		if (!animations.empty())
		{
			spriteRenderer->LoadSprite(animations.at(currentAnimationID).spriteSource);

			SyncCurrentFrame();
		}
	}

	void SpriteAnimator::SyncCurrentFrame()
	{
		const auto& props = animations.at(currentAnimationID);

		const auto frameRect = GetFrameRect();
		const auto frameIndex = static_cast<int>(animState.currentFrame);

		spriteRenderer->SetSourceRect(RectI
			{
				.x = frameRect.w * frameIndex,
				.y = frameRect.h * props.sourceRowNumber,
				.w = frameRect.w,
				.h = frameRect.h
			});

		spriteRenderer->SetDrawSize(Vector2I(frameRect.w, frameRect.h));
		spriteRenderer->SetFlipState(animState.flipState);
	}


	void SpriteAnimator::Init()
	{
		auto gameObject = GetGameObject();

		transform = Guard::AgainstNullAssignment(gameObject->GetTransform(), NAME_OF(transform));
		textureManager = Guard::AgainstNullAssignment(gameObject->CoreContext().textureManager, NAME_OF(textureManager));

		// A SpriteRenderer sibling isn't required to be present yet: it may not exist
		// at all, or it may be added later (see OnNewComponentAddedHandler below).
		spriteRenderer = gameObject->GetComponent<SpriteRenderer>();

		if (spriteRenderer != nullptr && !animations.empty())
		{
			spriteRenderer->LoadSprite(animations.at(currentAnimationID).spriteSource);

			SyncCurrentFrame();
		}
	}

	void SpriteAnimator::Update(float dt)
	{
		if (spriteRenderer == nullptr || animations.empty())
			return;

		auto spriteTexture = spriteRenderer->GetTexture();

		if (!spriteTexture)
		{
			// `sprite` may have been bound reactively (OnNewComponentAddedHandler) before
			// its own deferred Init() ran, so LoadSprite silently no-op'd back then.
			// Retry every tick until the SpriteRenderer is actually ready to load.
			spriteRenderer->LoadSprite(animations.at(currentAnimationID).spriteSource);

			spriteTexture = spriteRenderer->GetTexture();

			if (!spriteTexture)
				return;

			SyncCurrentFrame();
		}

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

		SyncCurrentFrame();
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

			if (spriteRenderer != nullptr)
			{
				spriteRenderer->LoadSprite(properties.spriteSource);

				SyncCurrentFrame();
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

		currentAnimationID = name;
		animState = SpriteAnimationState{};

		if (spriteRenderer == nullptr)
			return;

		spriteRenderer->LoadSprite(it->second.spriteSource);

		SyncCurrentFrame();
	}

	bool SpriteAnimator::IsPlaying(const std::string& name) const
	{
		return currentAnimationID == name;
	}

	void SpriteAnimator::SetFlipState(RenderFlip flipState)
	{
		animState.flipState = flipState;

		if (spriteRenderer != nullptr)
		{
			spriteRenderer->SetFlipState(flipState);
		}
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
		auto size = textureManager->GetTextureSize(spriteRenderer->GetTexture());

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