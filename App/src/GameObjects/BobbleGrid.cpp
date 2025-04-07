#include "GameObjects/BobbleGrid.h"
#include "Prefabs/Bobble.h"
#include <random>
#include <SDL_rect.h>


std::array<int, 160> grid = {
	61, 61, 71, 71, 51, 51, 41, 41, 31, 31,
	61, 61, 71, 71, 51, 51, 41, 41, 31, 0,
	21, 21, 11, 11, 1, 1, 61, 61, 71, 71,
	21, 11, 11, 1, 1, 61, 61, 71, 71, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

BobbleGrid::BobbleGrid()
{
	bobbles.clear();
}

void BobbleGrid::CreateBobbleRow(int rowNumber)
{
	static std::random_device rd;  // Seed for random number engine
	static std::mt19937 gen(rd()); // Mersenne Twister PRNG
	static std::uniform_int_distribution<int> dist(0, static_cast<int>(BobbleColor::ALL_COLOURS) - 1);

	auto tileSize = 32;

	auto displacement = rowNumber % 2 == 0 ? 0 : tileSize / 2;
	auto start = rowNumber * 10;
	auto end = rowNumber % 2 == 0 ? start + 10 - 1  : start + 10 - 2;

	auto placement = SDL_Rect
	{
		15,
		5,
		10,
		16
	};

	for (int i = start; i <= end; i++)
	{
		if (grid[i] <= 0)
			continue;

		auto x = placement.x * 32.0f + (i - start) * 32.0f + displacement;
		auto y = placement.y * 32.0f + rowNumber * 32.0f;

		GameObject::Instantiate<Bobble>(Vector2F{ x + tileSize / 2, y + tileSize / 2 }, static_cast<BobbleColor>(dist(gen)));
	}
}

void BobbleGrid::Init()
{
	GameObject::Init();

	for (int i = 0; i < 16; i++)
	{
		CreateBobbleRow(i);
	}
}