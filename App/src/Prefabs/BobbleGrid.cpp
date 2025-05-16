#include "Components/Transform.h"
#include "Models/BobbleGridLevel.h"
#include "Prefabs/Bobble.h"
#include "Prefabs/BobbleGrid.h"
#include <random>
#include <Tools/JsonSerializer.h>


BobbleGrid::BobbleGrid()
{
	transform = nullptr;

	if (!JsonSerializer::IsSerializable<BobbleGridLevel>())
	{
		throw std::runtime_error("BobbleGridLevel is not serializable.");
	}
}

void BobbleGrid::DestroyGridLevel()
{
	auto bobbles = OwningObject.lock()->GetChildren();

	for (auto& bobble : bobbles)
	{
		if (auto b = bobble.lock())
		{
			b->Destroy();
		}
	}

	bobbles.clear();
}

void BobbleGrid::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();
}

void BobbleGrid::Update(float deltaTime)
{
}

void BobbleGrid::Draw()
{
}

void BobbleGrid::SetNewGridLevel(std::string_view levelSource, int tileSize)
{
	DestroyGridLevel();

	auto levelData = JsonSerializer::DeserializeFromFile<BobbleGridLevel>(levelSource);

	static std::random_device rd;  // Seed for random number engine
	static std::mt19937 gen(rd()); // Mersenne Twister PRNG
	static std::uniform_int_distribution<int> dist(0, static_cast<int>(BobbleColor::ALL_COLOURS) - 1);

	auto placement = transform->GetWorldPosition();

	for (int i = 0; i < levelData.height; i++)
	{
		auto displacement = (i % 2) * tileSize / 2;
		auto start = i * levelData.width;
		auto end = start + (levelData.width - 1) - (i % 2);

		for (auto j = start; j <= end; j++)
		{
			if (levelData.grid[j] <= 0)
				continue;

			auto x = placement.x + (j - start) * tileSize + displacement;
			auto y = placement.y + i * tileSize;

			auto bobble = GameObject::Instantiate<Bobble>(Vector2F{ x + tileSize / 2.0f, y + tileSize / 2.0f }, static_cast<BobbleColor>(dist(gen)));

			OwningObject.lock()->AddChildGameObject(bobble);
		}
	}

	// TODO: Set bobble connection
}
