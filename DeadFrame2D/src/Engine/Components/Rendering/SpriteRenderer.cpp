#include "Constants/Rendering/DefaultSortOrders.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Engine/Components/Rendering/SpriteRenderer.h"
#include "Engine/Components/Transform.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	SpriteRenderer::SpriteRenderer(std::string_view texturePath)
	{
		spriteSize = Vector2I::One;

		LoadSprite(texturePath);

		renderTask.renderPhase = RenderPhase::WORLD;
		renderTask.sortOrder = DefaultSortOrders::SPRITE_RENDERER;
	}

	void SpriteRenderer::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));
	}

	void SpriteRenderer::Draw()
	{
		auto worldPosition = transform->GetWorldPosition();
		auto worldScale = transform->GetWorldScale();
		auto worldRotation = transform->GetWorldRotation();

		auto scaledDest = SDL_FRect
		{
			.x = worldPosition.x - (spriteSize.x * worldScale.x) / 2.0f,
			.y = worldPosition.y - (spriteSize.y * worldScale.y) / 2.0f,
			.w = spriteSize.x * worldScale.x,
			.h = spriteSize.y * worldScale.y
		};

		renderTask.renderData = SpriteRenderData
		{
			.texture = spriteTexture.get(),
			.srcRect = std::nullopt,
			.destRect = scaledDest,
			.rotation = worldRotation
		};

		RenderSystem::Submit(renderTask);
	}

	void SpriteRenderer::LoadSprite(std::string_view texturePath)
	{
		spriteTexture = TextureManager::LoadTexture(texturePath);

		SDL_QueryTexture(spriteTexture.get(), NULL, NULL, &spriteSize.x, &spriteSize.y);
	}

	std::shared_ptr<SDL_Texture> SpriteRenderer::GetTexture()
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
}