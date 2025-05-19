#include "Components/BobbleConnections.h"


BobbleConnections::BobbleConnections()
{
	for (size_t i = 0; i < BobbleConstants::MAX_BOBBLE_NEIGHBOURS; i++)
	{
		connectionList[i] = std::weak_ptr<GameObject>();
	}
}

void BobbleConnections::Init()
{
}

void BobbleConnections::Update(float deltaTime)
{
}

void BobbleConnections::Draw()
{
}

void BobbleConnections::SetConnectionAt(BobbleConnectionDirection connectionDirection, std::weak_ptr<GameObject> connection)
{
	auto index = static_cast<size_t>(connectionDirection);

	if (index >= BobbleConstants::MAX_BOBBLE_NEIGHBOURS || index < 0)
		return;

	connectionList[index] = connection;
}