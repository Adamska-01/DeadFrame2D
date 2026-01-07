#include "Core/SubSystems/Systems/Rendering/RenderSystem.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Engine/Components/Rendering/Sprite.h"
#include "Engine/Components/Transform.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	Sprite::Sprite(std::string_view texturePath)
	{
		spriteSize = Vector2I::One;

		LoadSprite(texturePath);

		renderTask.renderPhase = RenderPhase::WORLD;
		renderTask.sortOrder = 0;
	}

	void Sprite::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));
	}

	void Sprite::Draw()
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

	void Sprite::LoadSprite(std::string_view texturePath)
	{
		spriteTexture = TextureManager::LoadTexture(texturePath);

		SDL_QueryTexture(spriteTexture.get(), NULL, NULL, &spriteSize.x, &spriteSize.y);
	}

	std::shared_ptr<SDL_Texture> Sprite::GetTexture()
	{
		return spriteTexture;
	}

	int Sprite::GetSortOrder() const
	{
		return renderTask.sortOrder;
	}

	void Sprite::SetSortOrder(int sortOrder)
	{
		renderTask.sortOrder = sortOrder;
	}
}