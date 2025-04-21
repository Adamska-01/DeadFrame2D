#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "SubSystems/TextureManager.h"


Sprite::Sprite(std::string_view texturePath)
{
	transform = nullptr;
	destRect = { 0, 0, 0, 0 };

	LoadSprite(texturePath);
}

void Sprite::Init()
{
	transform = OwningObject->GetComponent<Transform>();
}

void Sprite::Update(float deltaTime)
{
	auto worldPosition = transform->GetWorldPosition();
	auto worldScale = transform->GetWorldScale();

	destRect.x = round(worldPosition.x - (destRect.w * worldScale.x) / 2);
	destRect.y = round(worldPosition.y - (destRect.h * worldScale.y) / 2);
}

void Sprite::Draw()
{
	if (!isActive)
		return;
	
	auto worldScale = transform->GetWorldScale();
	auto worldRotation = transform->GetWorldRotation();

	auto scaledDest = destRect;
	scaledDest.w *= worldScale.x;
	scaledDest.h *= worldScale.y;

	TextureManager::DrawTexture(spriteTexture, NULL, &scaledDest, worldRotation);
}

void Sprite::LoadSprite(std::string_view texturePath)
{
	spriteTexture = TextureManager::LoadTexture(texturePath);

	SDL_QueryTexture(spriteTexture.get(), NULL, NULL, &destRect.w, &destRect.h);
}

std::shared_ptr<SDL_Texture> Sprite::GetTexture()
{
	return spriteTexture;
}