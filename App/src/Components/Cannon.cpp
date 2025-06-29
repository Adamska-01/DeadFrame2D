#include "Components/BobbleGrid.h"
#include "Components/Cannon.h"
#include "Components/GameFlow/GameManager.h"
#include "Constants/AssetPaths.h"
#include "Events/LoadNewBobbleEvent.h"
#include "Prefabs/AudioClipObject.h"
#include "Prefabs/Bobble.h"
#include <algorithm>
#include <Components/BobbleController.h>
#include <Components/Collisions/Collider2D.h>
#include <Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h>
#include <Components/Physics/RigidBody2D.h>
#include <Components/Transform.h>
#include <Constants/PlayerInputConstants.h>
#include <Data/Collision/CollisionInfo.h>
#include <SubSystems/Input/Input.h>
#include <Tools/Helpers/Coroutines/CoroutineHelpers.h>
#include <Tools/Helpers/EventHelpers.h>
#include <Tools/Helpers/Guards.h>


Cannon::Cannon()
	: transform(nullptr),
	gameManager(nullptr),
	bobbleSpeed(20.0f),
	bounceProcessed(false),
	currentColor(0)
{
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(LoadNewBobbleEvent)), EventHelpers::BindFunction(this, &Cannon::LoadNewBobbleEventHandler), reinterpret_cast<std::uintptr_t>(this));
}

Cannon::~Cannon()
{
	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(LoadNewBobbleEvent)), reinterpret_cast<std::uintptr_t>(this));

	auto bobblePtr = loadedBobble.lock();
	 
	if (bobblePtr == nullptr)
		return;

	bobblePtr->Destroy();
}

BobbleColor Cannon::GetAvailableColor()
{
	auto availableColors = bobbleGrid->GetAvailableColors();

	// Default to black if no color is available (should never be the case)
	if (availableColors.size() <= 0)
		return BobbleColor::Black;

	while (true)
	{
		currentColor = ((currentColor + 1) % MAX_COLORS + MAX_COLORS) % MAX_COLORS;

		if (availableColors.contains((BobbleColor)currentColor))
			return (BobbleColor)currentColor;
	}
}

void Cannon::OnBobbleWallCollisionEnterHandler(const CollisionInfo& collisionInfo)
{
	if (bounceProcessed)
		return;

	if (collisionInfo.thisGameObject.expired()
		|| collisionInfo.otherGameObject.lock()->GetComponent<TiledMapCompatibleCollider2D>() == nullptr)
		return;

	auto bobbleController = collisionInfo.thisGameObject.lock()->GetComponent<BobbleController>();

	if (bobbleController == nullptr || !bobbleController->IsBobbleShot() || bobbleController->IsDestructionPending())
		return;

	auto rigidBody = collisionInfo.thisGameObject.lock()->GetComponent<RigidBody2D>();

	if (rigidBody == nullptr)
		return;

	auto currentVelocity = rigidBody->GetVelocity();
	auto normal = collisionInfo.normal;

	auto newVelocity = currentVelocity - normal * (2.0f * currentVelocity.Dot(normal));

	rigidBody->SetVelocity(newVelocity);

	bounceProcessed = true;
}

void Cannon::OnBobbleWallCollisionExitHandler(const CollisionInfo& collisionInfo)
{
	bounceProcessed = false;
}

void Cannon::LoadNewBobbleEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<LoadNewBobbleEvent>(dispatchableEvent);

	if (gameObjEvent == nullptr 
		|| gameObjEvent->interestedCannon == nullptr 
		|| gameObjEvent->interestedCannon != this)
		return;

	loadedBobble = LoadBobble();
	loadedBobble.lock()->GetTransform()->SetWorldPosition(transform->GetWorldPosition());

	auto rigidBody = loadedBobble.lock()->GetComponent<RigidBody2D>();

	if (rigidBody == nullptr)
		return;

	rigidBody->ChangeBodyType(BodyType2D::Dynamic);
}

void Cannon::ProcessRotationInput(float deltaTime)
{
	auto rotationDirection = 0.0f;

	if (Input::IsButtonHeld(PlayerInputSlot::PLAYER_1, "Left"))
	{
		rotationDirection = -1.0f;
	}
	if (Input::IsButtonHeld(PlayerInputSlot::PLAYER_1, "Right"))
	{
		rotationDirection = 1.0f;
	}

	transform->RotateByDegrees(rotationDirection * PlayerInputConstants::CANNON_ROTATION_SPEED * deltaTime);

	transform->SetLocalRotation(std::clamp(
		transform->GetLocalRotation(),
		PlayerInputConstants::CANNON_MIN_ROTATION,
		PlayerInputConstants::CANNON_MAX_ROTATION));
}

std::weak_ptr<GameObject> Cannon::ResetLoadedBobble()
{
	if (loadedBobble.lock() != nullptr)
	{
		loadedBobble.lock()->Destroy();
	}
	if (pendingBobble.lock() != nullptr)
	{
		pendingBobble.lock()->Destroy();
	}
	
	loadedBobble = CreateNewBobble(transform->GetWorldPosition());
	pendingBobble = CreateNewBobble(transform->GetWorldPosition() + (Vector2F::Left + Vector2F::Down / 2.0f).Normalize() * 60.0f);

	return loadedBobble;
}

std::weak_ptr<GameObject> Cannon::LoadBobble()
{
	auto availableColors = bobbleGrid->GetAvailableColors();

	if (loadedBobble.lock() == nullptr && pendingBobble.lock() == nullptr)
	{
		loadedBobble = CreateNewBobble(transform->GetWorldPosition());
		pendingBobble = CreateNewBobble(transform->GetWorldPosition() + (Vector2F::Left + Vector2F::Down / 2.0f).Normalize() * 60.0f);

		return loadedBobble;
	}

	auto loadedController = loadedBobble.lock()->GetComponent<BobbleController>();
	auto pendingController = pendingBobble.lock()->GetComponent<BobbleController>();

	if (loadedController->IsPartOfGrid())
	{
		loadedBobble = pendingBobble;

		loadedBobble.lock()->GetTransform()->SetWorldPosition(transform->GetWorldPosition());

		pendingBobble = CreateNewBobble(transform->GetWorldPosition() + (Vector2F::Left + Vector2F::Down / 2.0f).Normalize() * 60.0f);
	}

	return loadedBobble;
}

std::weak_ptr<GameObject> Cannon::CreateNewBobble(Vector2F startPos)
{
	auto newBobble = GameObject::Instantiate<Bobble>(startPos, GetAvailableColor());

	auto collider = newBobble.lock()->GetComponent<Collider2D>();

	if (collider != nullptr)
	{
		collider->RegisterContactEnterHandler(EventHelpers::BindFunction(this, &Cannon::OnBobbleWallCollisionEnterHandler), reinterpret_cast<uintptr_t>(this));
		collider->RegisterContactExitHandler(EventHelpers::BindFunction(this, &Cannon::OnBobbleWallCollisionExitHandler), reinterpret_cast<uintptr_t>(this));
	}

	auto rigidBody = newBobble.lock()->GetComponent<RigidBody2D>();

	if (rigidBody != nullptr)
	{
		rigidBody->ChangeBodyType(BodyType2D::Dynamic);
		rigidBody->SetVelocity(Vector2F::Zero);
	}

	return newBobble;
}

void Cannon::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();
	gameManager = OwningObject.lock()->GetComponentInParent<GameManager>();
	bobbleGrid = OwningObject.lock()->GetParent().lock()->GetComponentInChildren<BobbleGrid>();

	Tools::Helpers::GuardAgainstNull(transform, "Cannon::Init: Transform not found in the scene.");
	Tools::Helpers::GuardAgainstNull(gameManager, "Cannon::Init: GameManager not found in the scene.");
	Tools::Helpers::GuardAgainstNull(bobbleGrid, "Cannon::Init: BobbleGrid not found in the scene.");
}

void Cannon::Start()
{

}

void Cannon::Update(float deltaTime)
{
	if (!gameManager->IsInputAllowed())
		return;

	ProcessRotationInput(deltaTime);

	ShootBobble();
}

void Cannon::Draw()
{
}

void Cannon::ShootBobble()
{
	if (!Input::IsButtonPressed(PlayerInputSlot::PLAYER_1, "Jump"))
		return;

	auto bobblePtr = loadedBobble.lock();
	if (bobblePtr == nullptr)
		return;
	
	auto bobbleController = bobblePtr->GetComponent<BobbleController>();

	if (bobbleController == nullptr || bobbleController->IsBobbleShot())
		return;

	auto forward = transform->GetForward().Rotated(-90.0f);

	bobblePtr->GetComponent<BobbleController>()->ShootBobble(forward * bobbleSpeed);

	auto soundSourceObj = GameObject::Instantiate<AudioClipObject>(
		AssetPaths::Audio::BOBBLE_SHOT,
		Vector2F::Zero,
		0.4f,
		true);

	CoroutineScheduler::StartCoroutine(soundSourceObj.lock()->Destroy(1.0f));
}