#include "Components/BobbleController.h"
#include <Components/SpriteAnimator.h>
#include <GameObject.h>


BobbleController::BobbleController(BobbleColor bobbleColor)
	: partOfGrid(false),
	bobbleColor(bobbleColor),
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

	SetColor(bobbleColor);
}

void BobbleController::Update(float deltaTime)
{
}

void BobbleController::Draw()
{
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

	spriteAnimator->SetProp(false,(int)newColor, 10, (int)BobbleColor::ALL_COLOURS, 0);
}

void BobbleController::SetPartOfGrid(bool partOfGrid)
{
	this->partOfGrid = partOfGrid;
}