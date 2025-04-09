#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "SubSystems/TextureManager.h"


Sprite::Sprite(std::string texturePath)
{
	transform = nullptr;
	destRect = { 0, 0, 0, 0 };

	LoadSprite(texturePath);
}

void Sprite::Init()
{
	transform = OwningObject->GetComponent<Transform>();
}

void Sprite::Update(float dt)
{
	destRect.x = round(transform->position.x - (destRect.w * transform->scale.x) / 2);
	destRect.y = round(transform->position.y - (destRect.h * transform->scale.y) / 2);
}

void Sprite::Draw()
{
	if (!enableDraw)
		return;

	auto scaledDest = destRect;
	scaledDest.w *= transform->scale.x;
	scaledDest.h *= transform->scale.y;

	TextureManager::DrawTexture(spriteTexture.get(), NULL, &scaledDest, transform->angle);

#if _DEBUG 
	//Vector2 norm(transform->position.x + circleCollider.GetCircle().radius * cos(5 * MathConstants::PI / 3), transform->position.y + circleCollider.GetCircle().radius * sin(5 * MathConstants::PI / 3));
	//SDL_RenderDrawLine(Renderer::GetInstance()->GetRenderer(), transform->position.x, transform->position.y, norm.x, norm.y);
#endif
}

void Sprite::LoadSprite(std::string texturePath)
{
	spriteTexture = TextureManager::LoadTexture(texturePath);

	SDL_QueryTexture(spriteTexture.get(), NULL, NULL, &destRect.w, &destRect.h);
}

SDL_Texture* Sprite::GetTexture()
{
	return spriteTexture.get();
}
