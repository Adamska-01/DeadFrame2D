#include "Components/BobbleConnections.h"
#include "Components/Transform.h"
#include "Math/Vector2.h"
#include "Models/BobbleGridLevel.h"
#include "Prefabs/Bobble.h"
#include "Prefabs/BobbleGrid.h"
#include <random>
#include <Tools/JsonSerializer.h>


BobbleGrid::BobbleGrid()
{
	transform = nullptr;

	positionToBobble.clear();
	bobbleToPosition.clear();

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

	// TODO: Set the color in the grid data source itself
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
			auto coord = std::make_pair(i, j % levelData.width);
			
			positionToBobble[coord] = bobble;
			bobbleToPosition[bobble] = coord;

			OwningObject.lock()->AddChildGameObject(bobble);
		}
	}

	auto getNeighborCoord = [&](int row, int col, BobbleConnectionDirection dir) -> std::optional<std::pair<int, int>> 
		{
			auto isOddRow = (row % 2) != 0;
			switch (dir)
			{
			case BobbleConnectionDirection::TOP_LEFT:
				return std::make_pair(row - 1, isOddRow ? col : col - 1);
			case BobbleConnectionDirection::TOP_RIGHT:
				return std::make_pair(row - 1, isOddRow ? col + 1 : col);
			case BobbleConnectionDirection::LEFT:
				return std::make_pair(row, col - 1);
			case BobbleConnectionDirection::RIGHT:
				return std::make_pair(row, col + 1);
			case BobbleConnectionDirection::BOTTOM_LEFT:
				return std::make_pair(row + 1, isOddRow ? col : col - 1);
			case BobbleConnectionDirection::BOTTOM_RIGHT:
				return std::make_pair(row + 1, isOddRow ? col + 1 : col);
			default:
				return std::nullopt;
			}
		};
	
	for (const auto& [position, weakBobble] : positionToBobble)
	{
		const auto& [row, col] = position;
		auto bobbleConnections = weakBobble.lock()->GetComponent<BobbleConnections>();

		assert(bobbleConnections != nullptr && "Bobble has no 'BobbleConnections' component!");

		for (auto dir : 
			{
				BobbleConnectionDirection::TOP_LEFT,
				BobbleConnectionDirection::TOP_RIGHT,
				BobbleConnectionDirection::LEFT,
				BobbleConnectionDirection::RIGHT,
				BobbleConnectionDirection::BOTTOM_LEFT,
				BobbleConnectionDirection::BOTTOM_RIGHT 
			})
		{
			auto neighborCoordOpt = getNeighborCoord(row, col, dir);
			
			if (!neighborCoordOpt.has_value())
				continue;
			
			auto it = positionToBobble.find(neighborCoordOpt.value());
			
			if (it == positionToBobble.end())
				continue;

			bobbleConnections->SetConnectionAt(dir, it->second);
		}
	}
}
