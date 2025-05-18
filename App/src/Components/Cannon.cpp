#include "Components/Cannon.h"
#include "Prefabs/Bobble.h"
#include <cassert>
#include <Components/Collisions/Collider2D.h>
#include <Components/Physics/RigidBody2D.h>
#include <Components/Transform.h>
#include <Data/Collision/CollisionInfo.h>
#include <SubSystems/Input/Input.h>
#include <Tools/Helpers/EventHelpers.h>


Cannon::Cannon()
{
	transform = nullptr;
	bounceProcessed = false;
}

Cannon::~Cannon()
{
	auto bobblePtr = loadedBobble.lock();

	if (bobblePtr == nullptr)
		return;

	bobblePtr->Destroy();
}

void Cannon::OnBobbleWallCollisionEnterHandler(const CollisionInfo& collisionInfo)
{
	if (bounceProcessed)
		return;

	if (collisionInfo.thisCollider == nullptr || collisionInfo.thisCollider->GetGameObject().lock() == nullptr)
		return;

	auto rigidBody = collisionInfo.thisCollider->GetGameObject().lock()->GetComponent<RigidBody2D>();
	auto rigidBody2 = collisionInfo.otherCollider->GetGameObject().lock()->GetComponent<RigidBody2D>();

	if (rigidBody == nullptr)
		return;

	auto currentVelocity = rigidBody->GetVelocity();
	auto currentVelocity2 = rigidBody2->GetVelocity();
	auto normal = collisionInfo.normal;

	std::cout << normal << std::endl;

	auto newVelocity = currentVelocity - normal * (2 * currentVelocity.Dot(normal));

	rigidBody->SetVelocity(newVelocity);

	bounceProcessed = true;
}

void Cannon::OnBobbleWallCollisionExitHandler(const CollisionInfo& collisionInfo)
{
	bounceProcessed = false;
}

void Cannon::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();

	assert(transform != nullptr && "Cannon Object must have a Transform component.");
}

void Cannon::Update(float deltaTime)
{
	LoadBobble();

	if (Input::IsButtonPressed(PlayerInputSlot::PLAYER_1, "Jump"))
	{
		ShootBobble();
	}
}

void Cannon::Draw()
{
}

void Cannon::ShootBobble()
{
	auto bobblePtr = loadedBobble.lock();
	if (bobblePtr == nullptr)
		return;

	auto forward = transform->GetForward().Rotated(-90.0f);

	bobblePtr->GetComponent<RigidBody2D>()->SetVelocity(forward * 15.0f);
}

void Cannon::LoadBobble()
{
	if (loadedBobble.lock() != nullptr)
		return;

	// TODO: For now, randomize color. For later, get the color pick from the currently available colors in the grid.
	loadedBobble = GameObject::Instantiate<Bobble>(transform->GetWorldPosition(), BobbleColor::Blue);

	auto rigidBody = loadedBobble.lock()->GetComponent<RigidBody2D>();

	if (rigidBody == nullptr)
		return;

	rigidBody->ChangeBodyType(BodyType2D::Dynamic);

	auto collider = loadedBobble.lock()->GetComponent<Collider2D>();

	if (collider == nullptr)
		return;

	collider->RegisterCollisionEnterHandler(EventHelpers::BindFunction(this, &Cannon::OnBobbleWallCollisionEnterHandler), reinterpret_cast<uintptr_t>(this));
	collider->RegisterCollisionExitHandler(EventHelpers::BindFunction(this, &Cannon::OnBobbleWallCollisionExitHandler), reinterpret_cast<uintptr_t>(this));
}