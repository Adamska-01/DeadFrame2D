#include "Prefabs/Bobble.h"
#include "Prefabs/BobbleGrid.h"
#include <random>
#include <Tools/JsonSerializer.h>


BobbleGrid::BobbleGrid(std::string_view levelSource, Vector2F placement, int tileSize)
{
	bobbles.clear();

	this->placement = placement;
	this->tileSize = tileSize;

	if (!JsonSerializer::IsSerializable<BobbleGridLevel>())
	{
		throw std::runtime_error("BobbleGridLevel is not serializable.");
	}

	levelData = JsonSerializer::DeserializeFromFile<BobbleGridLevel>(levelSource);
}

void BobbleGrid::CreateBobbleRow(int row, int columnCount)
{
	static std::random_device rd;  // Seed for random number engine
	static std::mt19937 gen(rd()); // Mersenne Twister PRNG
	static std::uniform_int_distribution<int> dist(0, static_cast<int>(BobbleColor::ALL_COLOURS) - 1);

	auto displacement = (row % 2) * tileSize / 2;
	auto start = row * columnCount;
	auto end = start + (columnCount - 1) - (row % 2);

	for (auto i = start; i <= end; i++)
	{
		if (levelData.grid[i] <= 0)
			continue;

		auto x = placement.x + (i - start) * tileSize + displacement;
		auto y = placement.y + row * tileSize;

		GameObject::Instantiate<Bobble>(Vector2F{ x + tileSize / 2.0f, y + tileSize / 2.0f }, static_cast<BobbleColor>(dist(gen)));
	}
}

void BobbleGrid::Init()
{
	GameObject::Init();

	for (int i = 0; i < levelData.height; i++)
	{
		CreateBobbleRow(i, levelData.width);
	}
}