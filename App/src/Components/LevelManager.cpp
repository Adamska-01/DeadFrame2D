#include "Components/LevelManager.h"
#include <Components/GameMapParser.h>
#include <Constants/TiledObjectGroupNames.h>
#include <Management/SceneManager.h>
#include <TileEditors/Tiled/Models/TiledObjectGroup.h>
#include <Tools/IO/FileSystemUtils.h>


LevelManager::LevelManager(GameMode gameMode)
	: gameMode(gameMode), 
	bobbleGrid()
{
	levelSources = FileSystemUtils::GetFilesInFolder("App/Assets/Levels");
}

LevelManager::~LevelManager()
{
	for (const auto& grid : bobbleGrid)
	{
		auto gridPtr = grid.lock();
		
		if (gridPtr == nullptr)
			continue;

		gridPtr->Destroy();
	}
}

void LevelManager::Init()
{
	auto gameMapParser = SceneManager::FindObjectOfType<GameMapParser>();

	if (gameMapParser == nullptr)
	{
		throw std::runtime_error("BobbleGrid::Init: GameMapParser not found in the scene.");
	}

	auto tileSize = gameMapParser->GetTileSize();
	auto placements = gameMapParser->RetrieveObjectGroup(TiledObjectGroupNames::GRID_START_GROUP);

	if (!placements.has_value())
	{
		throw std::runtime_error("BobbleGrid::Init: Grid start object group not found.");
	}

	auto placement = placements.value().points.front();
	auto p1 = placements.value().points.front();
	auto p2 = placements.value().points.back();

	if (gameMode == GameMode::SINGLE_PLAYER)
	{
		bobbleGrid[0] = GameObject::Instantiate<BobbleGrid>(levelSources.front(), p1, tileSize);
	}
	else if (gameMode == GameMode::MULTIPLAYER)
	{
		bobbleGrid[0] = GameObject::Instantiate<BobbleGrid>(levelSources.front(), p1, tileSize);
		bobbleGrid[1] = GameObject::Instantiate<BobbleGrid>(levelSources.back(), p2, tileSize);
	}
}

void LevelManager::Update(float dt)
{
}

void LevelManager::Draw()
{
}
