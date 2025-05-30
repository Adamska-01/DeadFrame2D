#include "Components/BobbleGrid.h"
#include "Components/Cannon.h"
#include "Components/GameFlow/GameManager.h"
#include "Components/GameMapParser.h"
#include "Constants/AssetPaths.h"
#include "Constants/TiledObjectGroupNames.h"
#include <Components/Transform.h>
#include <Management/SceneManager.h>
#include <TileEditors/Tiled/Models/TiledObjectGroup.h>
#include <Tools/IO/FileSystemUtils.h>


GameManager::GameManager()
{
	levelSources = FileSystemUtils::GetFilesInFolder(AssetPaths::Levels::SINGLEPLAYER_LEVELS_FOLDER_PATH);
}

void GameManager::Init()
{
	// Get Map Parser
	auto gameMapParser = SceneManager::FindObjectOfType<GameMapParser>();
	
	if (gameMapParser == nullptr)
	{
		throw std::runtime_error("Cannon::Init: GameMapParser not found in the scene.");
	}

	// Get Player Position
	auto groupObject = gameMapParser->RetrieveObjectGroup(TiledObjectGroupNames::PLAYER_POSITION_GROUP);

	if (!groupObject.has_value())
	{
		throw std::runtime_error("GameManager::Init: Player position group not found.");
	}

	auto player = OwningObject.lock()->GetComponentInChildren<Cannon>();
	player->GetGameObject().lock()->GetComponent<Transform>()->SetWorldPosition(groupObject.value().points.front());

	// Generate first level
	auto tileSize = gameMapParser->GetTileSize();
	auto placements = gameMapParser->RetrieveObjectGroup(TiledObjectGroupNames::GRID_START_GROUP);

	if (!placements.has_value())
	{
		throw std::runtime_error("GameManager::Init: Grid start object group not found.");
	}

	auto bobbleGrid = OwningObject.lock()->GetComponentInChildren<BobbleGrid>();
	bobbleGrid->GetGameObject().lock()->GetComponent<Transform>()->SetWorldPosition(placements.value().points.front());
	bobbleGrid->SetNewGridLevel(levelSources.front(), tileSize);
}

void GameManager::Update(float deltaTime)
{
}

void GameManager::Draw()
{
}
