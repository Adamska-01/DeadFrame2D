#include "Bobble.h"
#include "Cannon.h"
#include <Components/Transform.h>
#include <Math/MathConstants.h>
#include <SDL.h>
#include <SubSystems/AudioManager.h>
#include <SubSystems/Input.h>
#include <SubSystems/Renderer.h>
#include <Utility/UsefullFunctions.h>


Cannon::Cannon(Vector2 pos, int whatPlayer)
{
	gameOver = false;

	WhatPlayer = whatPlayer;

	// Rotation 
	minRot = -1.35f;
	maxRot = 1.35f;
	rotationSpeed = 100;

	CannonT = new Transform();

	// Setpos
	transform->position = CannonT->position = pos;
	// Setting scaling
	transform->scale = transform->scale * 1.5f;
	
	// Get cannon w and h
	cannonTexture = TextureManager::GetInstance()->LoadTexture("Assets/Sprites/Cannon.png");
	SDL_QueryTexture(cannonTexture, NULL, NULL, &canRect.w, &canRect.h);

	//Set rot point for the arrow
	arrowTexture = TextureManager::GetInstance()->LoadTexture("Assets/Sprites/Arrow.png");
	SDL_QueryTexture(arrowTexture, NULL, NULL, &arrowRect.w, &arrowRect.h);
	rotPoint.x = arrowRect.w * transform->scale.x / 2;
	rotPoint.y = arrowRect.h * transform->scale.y / 2;

	//Rot pivot
	Vector2 p2(transform->position.x + ((canRect.w * CannonT->scale.x) / 2), transform->position.y + (canRect.h * CannonT->scale.y) / 2);
	RotationPos = p2;

	canRect.x = RotationPos.x - (canRect.w * CannonT->scale.x) / 2;
	canRect.y = RotationPos.y - (canRect.h * CannonT->scale.y) / 2;

	//Set Direction point -->
	auto s = sin(transform->angle);
	auto c = cos(transform->angle);

	Direction.x = c * (transform->position.x + ((canRect.w * CannonT->scale.x)/2) - RotationPos.x) - s * (transform->position.y - RotationPos.y) + RotationPos.x;
	Direction.y = s * (transform->position.x + ((canRect.w * CannonT->scale.x)/2) - RotationPos.x) + c * (transform->position.y - RotationPos.y) + RotationPos.y;

	//Set booble to rot pivot
	bobble = new Bobble(RotationPos);
}

Cannon::~Cannon()
{
	Clean();
}

void Cannon::Update(float dt)
{
	if (bobble != nullptr && bobble->CanDestroy())
	{
		delete bobble;
		bobble = nullptr;
	}

	if (gameOver)
		return;
	
	auto direction = 0;
	if (Input::GetInstance()->ButtonHeld((Players)WhatPlayer, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
	{
		direction = -1;
	}
	else if (Input::GetInstance()->ButtonHeld((Players)WhatPlayer, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
	{
		direction = 1;
	}

	if (direction != 0)
	{
		transform->RotateBy(direction * rotationSpeed * dt);
		transform->angle = UsefullFunctions::Clamp(transform->angle, minRot, maxRot);

		auto s = sin(transform->angle);
		auto c = cos(transform->angle);

		auto adjustedX = transform->position.x + ((canRect.w * CannonT->scale.x) / 2) - RotationPos.x;
		auto adjustedY = transform->position.y - RotationPos.y;

		Direction.x = c * adjustedX - s * adjustedY + RotationPos.x;
		Direction.y = s * adjustedX + c * adjustedY + RotationPos.y;
	}

	bobble = bobble != nullptr ? bobble : new Bobble(RotationPos);

	//Change colour
	if (Input::GetInstance()->ButtonPressed((Players)WhatPlayer, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) && !bobble->IsFiring() && !bobble->CanExplode())
	{
		bobble->ChangeColour(-1);
	}
	if (Input::GetInstance()->ButtonPressed((Players)WhatPlayer, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && !bobble->IsFiring() && !bobble->CanExplode())
	{
		bobble->ChangeColour(1);
	}

	//Shoot 
	if (Input::GetInstance()->ButtonPressed((Players)WhatPlayer, SDL_CONTROLLER_BUTTON_A) && !bobble->IsFiring())
	{
		auto normal = Direction - RotationPos;

		bobble->SetVelocity(normal.Normalize());
		bobble->Fire();

		AudioManager::GetInstance()->PlaySFX(3, 0, 0);
	}

	//Return to cannon
	if (bobble->GetCircle().position.y < 0)
	{
		bobble->StopFire();
		bobble->SetPosition(RotationPos);
	}

	bobble->Update(dt);
}

void Cannon::Draw()
{
#if _DEBUG
	SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);
#endif

	// Get destRect x and y from the rotation point 
	Vector2 arrow( RotationPos.x - (arrowRect.w * transform->scale.x) / 2, RotationPos.y - (arrowRect.h * transform->scale.y) / 2);

	// Draw cannon and arrow
	TextureManager::GetInstance()->Draw(cannonTexture, canRect, CannonT->scale);
	TextureManager::GetInstance()->DrawRotate(arrowTexture, arrow, transform->scale, transform->angle / (MathConstants::PI / 180), rotPoint);
	
#if _DEBUG
	SDL_RenderDrawLine(Renderer::GetInstance()->GetRenderer(), RotationPos.x, RotationPos.y, Direction.x, Direction.y);
#endif

	// Draw bobble
	if (bobble != nullptr) bobble->Draw();
}

void Cannon::Clean()
{
	if (bobble != nullptr)
	{
		delete bobble;
		bobble = nullptr;
	}
	if (CannonT != nullptr)
	{
		delete CannonT;
		CannonT = nullptr;
	}
	if (cannonTexture != nullptr)
	{
		SDL_DestroyTexture(cannonTexture);
		cannonTexture = nullptr;
	}
	if (arrowTexture != nullptr)
	{
		SDL_DestroyTexture(arrowTexture);
		arrowTexture = nullptr;
	}
}

void Cannon::SetGameover(bool value)
{ 
	gameOver = value; 
}

bool Cannon::GetGameOver()
{ 
	return gameOver; 
}

Bobble* Cannon::GetBobble()
{ 
	return bobble; 
}