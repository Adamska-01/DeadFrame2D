#pragma once
#include "GameComponent.h"
#include <memory>
#include <SDL.h>
#include <string>


class Transform;
struct SDL_Texture;


class Sprite : public GameComponent
{
private :
	std::shared_ptr<SDL_Texture> spriteTexture;

	Transform* transform;

	SDL_Rect destRect;


public:
	Sprite(std::string_view texturePath);
	
	Sprite(Sprite&& other) = default;

	virtual ~Sprite() override = default;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void LoadSprite(std::string_view texturePath);
	
	SDL_Texture* GetTexture();
};