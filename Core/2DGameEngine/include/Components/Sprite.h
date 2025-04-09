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
	Sprite(std::string texturePath);
	
	Sprite(Sprite&& other) = default;

	virtual ~Sprite() override = default;


	void Init();

	void Update(float dt);

	void Draw();


	void LoadSprite(std::string texturePath);
	
	SDL_Texture* GetTexture();
};