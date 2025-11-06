#include "Core/SubSystems/Systems/TextureManager.h"
#include "Engine/Components/Rendering/Sprite.h"
#include "Engine/Components/Transform.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Utilities;


	Sprite::Sprite(std::string_view texturePath)
	{
		spriteSize = Vector2I::One;

		LoadSprite(texturePath);
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

		auto scaledDest = SDL_Rect
		{
			static_cast<int>(worldPosition.x - (spriteSize.x * worldScale.x) / 2),
			static_cast<int>(worldPosition.y - (spriteSize.y * worldScale.y) / 2),
			static_cast<int>(spriteSize.x * worldScale.x),
			static_cast<int>(spriteSize.y * worldScale.y)
		};

		TextureManager::DrawTextureWorldSpace(spriteTexture, NULL, &scaledDest, worldRotation);
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
}