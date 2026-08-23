#include "Constants/Rendering/DefaultSortOrders.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
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


	SpriteRenderer::SpriteRenderer(std::string_view texturePath)
		: texturePath(texturePath)
	{
		spriteSize = Vector2I::One;

		renderTask.renderPhase = RenderPhase::WORLD;
		renderTask.sortOrder = DefaultSortOrders::SPRITE_RENDERER;
	}

	void SpriteRenderer::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));
		textureManager = GetGameObject()->CoreContext().textureManager;

		if (!texturePath.empty())
		{
			LoadSprite(texturePath);
		}
	}

	void SpriteRenderer::Draw()
	{
		auto worldPosition = transform->GetWorldPosition();
		auto worldScale = transform->GetWorldScale();
		auto worldRotation = transform->GetWorldRotation();

		auto drawSize = overrideDrawSize.value_or(spriteSize);

		auto scaledDest = RectF
		{
			.x = worldPosition.x - (drawSize.x * worldScale.x) / 2.0f,
			.y = worldPosition.y - (drawSize.y * worldScale.y) / 2.0f,
			.w = drawSize.x * worldScale.x,
			.h = drawSize.y * worldScale.y
		};

		renderTask.renderData = SpriteRenderData
		{
			.texture = spriteTexture,
			.srcRect = overrideSrcRect,
			.destRect = scaledDest,
			.flip = flipState,
			.rotation = worldRotation
		};

		RenderSystem::Submit(renderTask);
	}

	void SpriteRenderer::LoadSprite(std::string_view texturePath)
	{
		this->texturePath = texturePath;

		if (!textureManager)
			return;

		spriteTexture = textureManager->LoadTexture(texturePath);

		spriteSize = textureManager->GetTextureSize(spriteTexture);
	}

	TextureID SpriteRenderer::GetTexture()
	{
		return spriteTexture;
	}

	int SpriteRenderer::GetSortOrder() const
	{
		return renderTask.sortOrder;
	}

	void SpriteRenderer::SetSortOrder(int sortOrder)
	{
		renderTask.sortOrder = sortOrder;
	}

	std::optional<RectI> SpriteRenderer::GetSourceRect() const
	{
		return overrideSrcRect;
	}

	void SpriteRenderer::SetSourceRect(std::optional<RectI> srcRect)
	{
		overrideSrcRect = srcRect;
	}

	Vector2I SpriteRenderer::GetDrawSize() const
	{
		return overrideDrawSize.value_or(spriteSize);
	}

	void SpriteRenderer::SetDrawSize(std::optional<Vector2I> size)
	{
		overrideDrawSize = size;
	}

	RenderFlip SpriteRenderer::GetFlipState() const
	{
		return flipState;
	}

	void SpriteRenderer::SetFlipState(RenderFlip flip)
	{
		flipState = flip;
	}
}