#include "Components/BobbleController.h"


BobbleController::BobbleController()
	: itsHanging(false), partOfGrid(false)
{
	for (size_t i = 0; i < BobbleConstants::MAX_BOBBLE_NEIGHBOURS; i++)
	{
		connectionList[i] = std::weak_ptr<GameObject>();
	}
}

void BobbleController::Init()
{
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

bool BobbleController::IsPartOfGrid() const
{
	return partOfGrid;
}

bool BobbleController::IsHanging() const
{
	return itsHanging;
}

void BobbleController::SetHanging(bool hanging)
{
	itsHanging = hanging;
}

void BobbleController::SetPartOfGrid(bool partOfGrid)
{
	this->partOfGrid = partOfGrid;
}