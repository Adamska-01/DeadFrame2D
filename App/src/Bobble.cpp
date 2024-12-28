#include "Bobble.h"
#include <Components/Collisions/CollisionHandler.h>
#include <Components/Sprite.h>
#include <Components/SpriteAnimator.h>
#include <Components/Transform.h>
#include <Math/MathConstants.h>
#include <SubSystems/Renderer.h>


bool Bobble::RandInit = false;

const std::string IDLE_BOBBLE_PATH = "Assets/Sprites/IdleBobbles.png";

const std::string BLOW_BOBBLE_PATH = "Assets/Sprites/BlowBobbles.png";


Bobble::Bobble(Vector2 startPos)
{
	//Init Random seeed
	if (!RandInit)
	{
		srand((unsigned int)time(NULL));
		RandInit = true;
	}

	//assign start pos to both bubble and collider, and scale
	transform->position = startPos;
	circleCollider.SetPos(startPos);
	transform->scale = transform->scale * 2.0f;
	
	//initialize Velocity and speed
	Velocity = Vector2::Zero;
	speed = 500.0f;

	isFired = false;
	canExplode = false;
	canDestroy = false;
	colour = static_cast<Colour>(rand() % (int)Colour::Purple);

	sprite = &AddComponent<Sprite>(IDLE_BOBBLE_PATH);

	//Animations
	animator = &AddComponent<SpriteAnimator>();
	animator->SetProp(true, (int)colour, 10, (int)Colour::ALL_COLOURS, 10);

	//set the width and height of the bobble
	dstRect = animator->GetFrameRect();

	//set circle collider size
	circleCollider.SetSize(((float)dstRect.h * transform->scale.y) / 2);
}

void Bobble::StopFire()
{
	isFired = false;
}

void Bobble::SetVelocity(Vector2 v)
{
	Velocity = v;
}

void Bobble::SetPosition(Vector2 v)
{
	transform->position = v;
	circleCollider.SetPos(v);
}

void Bobble::SetDestruction(bool value)
{
	canExplode = value;
}

void Bobble::SetSpeed(float value)
{
	speed = value;
}

Circle Bobble::GetCircle()
{
	return circleCollider.GetCircle();
}

bool Bobble::CanDestroy()
{
	return canDestroy;
}

bool Bobble::CanExplode()
{ 
	return canExplode; 
}

bool Bobble::IsFiring()
{ 
	return isFired; 
}

Colour Bobble::GetColor()
{ 
	return colour; 
}

void Bobble::ChangeColour(int changeValue)
{
	//Set colour
	colour = static_cast<Colour>((int)colour + changeValue);

	//Check the colour is in the range
	if (colour > Colour::Orange)
	{
		colour = Colour::Black;
	}
	else if (colour < Colour::Black)
	{
		colour = Colour::Orange;
	}

	//Set animation's prop based on the colour
	animator->SetProp(true, (int)colour, 10, (int)Colour::ALL_COLOURS, 5);
}

Bobble::~Bobble()
{
	Clean();
}

void Bobble::Update(float dt)
{
	GameObject::Update(dt);

	//Motion
	if (isFired)
	{
		transform->Translate(Velocity * speed * dt);
		circleCollider.SetPos(transform->position);
	}

	AnimationStateMachine();

	//Updates Animation
	animator->Update(dt);
}

void Bobble::Draw()
{
	GameObject::Draw();

	animator->Draw();

#if _DEBUG //Prints the bobble's radius (line)
	Vector2 norm(transform->position.x + circleCollider.GetCircle().radius * cos(5 * MathConstants::PI / 3), transform->position.y + circleCollider.GetCircle().radius * sin( 5 * MathConstants::PI / 3 ));
	SDL_RenderDrawLine(Renderer::GetInstance()->GetRenderer(), transform->position.x, transform->position.y, norm.x, norm.y);
#endif
}

void Bobble::Clean()
{
}

void Bobble::AnimationStateMachine()
{
	sprite->LoadSprite(canExplode ? BLOW_BOBBLE_PATH : IDLE_BOBBLE_PATH);
}

bool Bobble::CollisionsWithWall(SDL_Rect wall)
{
	if (CollisionHandler::CircleToBoxCollision(circleCollider.GetCircle(), wall, SurfacePoint))
	{
		auto Normal = transform->position - SurfacePoint; //Surface normal (vector subtraction)

		Normal.Normalize(); //Normalize vector

		Bounce(Normal);

		return true;
	}
	
	return false;
}

bool Bobble::CollisionsWithBall(Circle ball)
{
	return CollisionHandler::CircleToCircleCollision(circleCollider.GetCircle(), ball);
}

void Bobble::Bounce(Vector2 Normal)
{
	//Reflection formula
	//reflect = velocity - 2*(v DOT normal) * normal
	Velocity = Velocity - Normal * (2 * Velocity.Dot(Normal));
}

void Bobble::Fire()
{
	isFired = true;
}