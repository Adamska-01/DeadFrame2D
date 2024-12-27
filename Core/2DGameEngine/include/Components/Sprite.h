#pragma once
#include "GameComponent.h"
#include <string>
#include <SDL.h>


class Transform;
struct SDL_Texture;


class Sprite : public GameComponent
{
private :
	SDL_Texture* spriteTexture;

	Transform* transform;

	SDL_Rect destRect;


public:
	Sprite(std::string texturePath);
	
	Sprite(Sprite&& other) = default;

	~Sprite() override;


	void Init();

	void Update(float dt);

	void Draw();


	void LoadSprite(std::string texturePath);
	
	SDL_Texture* GetTexture();


protected:
	virtual void Clean();
};