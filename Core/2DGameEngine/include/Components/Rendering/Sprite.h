#pragma once
#include "Components/GameComponent.h"
#include "Math/Vector2.h"
#include <memory>
#include <string>


class Transform;
struct SDL_Texture;


class Sprite : public GameComponent
{
protected:
	std::shared_ptr<SDL_Texture> spriteTexture;

	Transform* transform;

	Vector2I spriteSize;


public:
	Sprite(std::string_view texturePath);
	
	Sprite(Sprite&& other) = default;

	virtual ~Sprite() override = default;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void LoadSprite(std::string_view texturePath);
	
	std::shared_ptr<SDL_Texture> GetTexture();
};