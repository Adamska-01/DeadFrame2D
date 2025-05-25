#include "Components/BobbleController.h"
#include "Constants/AssetPaths.h"
#include <cassert>
#include <Components/Rendering/Sprite.h>
#include <Components/SpriteAnimator.h>
#include <GameObject.h>


BobbleController::BobbleController(BobbleColor bobbleColor)
	: partOfGrid(false),
	pendingDestruction(false),
	bobbleColor(bobbleColor),
	sprite(nullptr),
	spriteAnimator(nullptr)
{
	for (size_t i = 0; i < BobbleConstants::MAX_BOBBLE_NEIGHBOURS; i++)
	{
		connectionList[i] = std::weak_ptr<GameObject>();
	}
}

void BobbleController::Init()
{
	spriteAnimator = OwningObject.lock()->GetComponent<SpriteAnimator>();
	sprite = OwningObject.lock()->GetComponent<Sprite>();

	assert(sprite != nullptr && "BobbleController needs a Sprite component.");
	assert(spriteAnimator != nullptr && "BobbleController needs a spriteAnimator component.");

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

bool BobbleController::IsPartOfGrid() const
{
	return partOfGrid;
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