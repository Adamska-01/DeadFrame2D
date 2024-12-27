#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "SubSystems/TextureManager.h"
#include <Math/MathConstants.h>
#include <SubSystems/Renderer.h>


Sprite::Sprite(std::string texturePath)
{
	spriteTexture = nullptr;
	transform = nullptr;
	destRect = { 0, 0, 0, 0 };

	LoadSprite(texturePath);
}

Sprite::~Sprite()
{
	Clean();
}

void Sprite::Init()
{
	transform = &OwningObject->GetComponent<Transform>();
}

void Sprite::Update(float dt)
{
	// This is probably wrong. Why not just using the transform to determine the destination?
	destRect.x = round(transform->position.x - ((float)destRect.h * transform->scale.y) / 2);
	destRect.y = round(transform->position.y - ((float)destRect.h * transform->scale.y) / 2);
}

void Sprite::Draw()
{
	if (!enableDraw)
		return;

	TextureManager::GetInstance()->Draw(spriteTexture, destRect, transform->scale);

#if _DEBUG //Prints the bobble's radius (line)
	//Vector2 norm(transform->position.x + circleCollider.GetCircle().radius * cos(5 * MathConstants::PI / 3), transform->position.y + circleCollider.GetCircle().radius * sin(5 * MathConstants::PI / 3));
	//SDL_RenderDrawLine(Renderer::GetInstance()->GetRenderer(), transform->position.x, transform->position.y, norm.x, norm.y);
#endif
}

void Sprite::LoadSprite(std::string texturePath)
{
	Clean();

	spriteTexture = TextureManager::GetInstance()->LoadTexture(texturePath);

	SDL_QueryTexture(spriteTexture, NULL, NULL, &destRect.w, &destRect.h);
}

SDL_Texture* Sprite::GetTexture()
{
	return spriteTexture;
}

void Sprite::Clean()
{
	if (spriteTexture == nullptr)
		return;

	SDL_DestroyTexture(spriteTexture);
	spriteTexture = nullptr;
}