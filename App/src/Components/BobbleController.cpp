#include "Components/BobbleController.h"
#include "Constants/AssetPaths.h"
#include <Components/Collisions/Collider2D.h>
#include <Components/Physics/RigidBody2D.h>
#include <Components/Rendering/Sprite.h>
#include <Components/SpriteAnimator.h>
#include <Components/Transform.h>
#include <GameObject.h>
#include <Tools/Helpers/Guards.h>


BobbleController::BobbleController(BobbleColor bobbleColor)
	: partOfGrid(false),
	pendingDestruction(false),
	isDropping(false),
	isShot(false),
	bobbleColor(bobbleColor),
	transform(nullptr),
	sprite(nullptr),
	spriteAnimator(nullptr),
	rigidBody(nullptr),
	collider(nullptr)
{
	for (size_t i = 0; i < BobbleConstants::MAX_BOBBLE_NEIGHBOURS; i++)
	{
		connectionList[i] = std::weak_ptr<GameObject>();
	}
}

void BobbleController::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();
	sprite = OwningObject.lock()->GetComponent<Sprite>();
	spriteAnimator = OwningObject.lock()->GetComponent<SpriteAnimator>();
	rigidBody = OwningObject.lock()->GetComponent<RigidBody2D>();
	collider = OwningObject.lock()->GetComponent<Collider2D>();

	Tools::Helpers::GuardAgainstNull(transform, "BobbleController needs a Transform component.");
	Tools::Helpers::GuardAgainstNull(sprite, "BobbleController needs a Sprite component.");
	Tools::Helpers::GuardAgainstNull(spriteAnimator, "BobbleController needs a SpriteAnimator component.");
	Tools::Helpers::GuardAgainstNull(rigidBody, "BobbleController needs a RigidBody2D component.");
	Tools::Helpers::GuardAgainstNull(collider, "BobbleController needs a Collider2D component.");

	SetColor(bobbleColor);
}

void BobbleController::Update(float deltaTime)
{
	if (!pendingDestruction)
		return;

	auto popAnimationProgress = spriteAnimator->GetAnimationProgressRatio();

	if (popAnimationProgress < 1.0f)
		return;

	OwningObject.lock()->Destroy();
}

void BobbleController::Draw()
{
}

void BobbleController::ShootBobble(Vector2F shootDirection)
{
	rigidBody->SetVelocity(shootDirection);

	isShot = true;
}

void BobbleController::PopBobble()
{
	sprite->LoadSprite(AssetPaths::Sprites::POP_BOBBLE_PATH);

	auto properties = SpriteAnimationProperties
	{
		.loop = false,
		.animSpeed = 15.0f,
		.sourceRowNumber = (int)bobbleColor,
		.columnCount = 7,
		.rowCount = (int)BobbleColor::ALL_COLOURS,
	};

	spriteAnimator->SetAnimationProperties(properties);

	pendingDestruction = true;
}

void BobbleController::DropBobble()
{
	rigidBody->ChangeBodyType(BodyType2D::Dynamic);
	rigidBody->SetGravityScale(5.0f);

	collider->SetIsTrigger(true);

	isDropping = true;
}

void BobbleController::SetConnectionAt(BobbleConnectionDirection connectionDirection, std::weak_ptr<GameObject> connection)
{
	auto index = static_cast<size_t>(connectionDirection);

	if (index >= BobbleConstants::MAX_BOBBLE_NEIGHBOURS || index < 0)
		return;

	connectionList[index] = connection;
}

BobbleColor BobbleController::GetBobbleColor() const
{
	return bobbleColor;
}

bool BobbleController::IsDestructionPending() const
{
	return !pendingDestruction && !isDropping;
}

bool BobbleController::IsPartOfGrid() const
{
	return partOfGrid;
}

bool BobbleController::IsBobbleShot() const
{
	return isShot;
}

std::weak_ptr<GameObject> BobbleController::GetConnectionAt(BobbleConnectionDirection connectionDirection) const
{
	auto index = static_cast<size_t>(connectionDirection);

	if (index >= BobbleConstants::MAX_BOBBLE_NEIGHBOURS || index < 0)
		return std::weak_ptr<GameObject>();

	return connectionList[index];
}

void BobbleController::SetColor(BobbleColor newColor)
{
	bobbleColor = newColor;

	if (spriteAnimator == nullptr)
		return;

	auto properties = SpriteAnimationProperties
	{
		.loop = false,
		.animSpeed = 0.0f,
		.sourceRowNumber = (int)newColor,
		.columnCount = 10,
		.rowCount = (int)BobbleColor::ALL_COLOURS,
	};

	spriteAnimator->SetAnimationProperties(properties);
}

void BobbleController::SetPartOfGrid(bool partOfGrid)
{
	this->partOfGrid = partOfGrid;
}