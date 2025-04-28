#include "Components/Rendering/Sprite.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "SubSystems/TextureManager.h"


Sprite::Sprite(std::string_view texturePath)
{
	transform = nullptr;
	spriteSize = Vector2I::One;

	LoadSprite(texturePath);
}

void Sprite::Init()
{
	transform = OwningObject->GetComponent<Transform>();
}

void Sprite::Update(float deltaTime)
{
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

	TextureManager::DrawTexture(spriteTexture, NULL, &scaledDest, worldRotation);
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