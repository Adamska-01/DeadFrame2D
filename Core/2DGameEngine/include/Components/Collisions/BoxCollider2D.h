#pragma once
#include "Components/GameComponent.h"
#include "SubSystems/Renderer.h"
#include <SDL.h>


class Transform;


class BoxCollider2D : GameComponent
{
private:
	SDL_Rect box;

	SDL_Rect cropOffset;

	Transform* transform;


public:
	BoxCollider2D(SDL_Rect box, SDL_Rect cropOffset);

	~BoxCollider2D() = default;


	SDL_Rect GetCollisionBox();

	void SetBuffer(int x, int y, int w, int h);

	void SetBox(int x, int y, int w, int h);

	// TODO: Make a damn factory for this, and draw the bow only when running on debug
	void DrawBox(Uint8 r, Uint8 g, Uint8 b, Uint8 a);


	virtual void Init() override;

	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;
};