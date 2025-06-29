#include "Components/BobbleGrid.h"
#include "Components/Cannon.h"
#include "Components/GameFlow/GameManager.h"
#include "Components/GameMapParser.h"
#include "Constants/AssetPaths.h"
#include "Constants/BobbleConstants.h"
#include "Constants/TiledObjectGroupNames.h"
#include <Components/GridCeiling.h>
#include <Components/MenuManager.h>
#include <Components/Transform.h>
#include <Components/UI/LevelTextController.h>
#include <Coroutines/CoroutineScheduler.h>
#include <Management/SceneManager.h>
#include <TileEditors/Tiled/Models/TiledObjectGroup.h>
#include <Tools/Helpers/Guards.h>
#include <Tools/IO/FileSystemUtils.h>
#include <Components/Audio/AudioSource.h>


GameManager::GameManager()
	: tileSize(32),
	currentLevelIndex(-1),
	player(nullptr),
	bobbleGrid(nullptr),
	gridCeiling(nullptr),
	menuManager(nullptr),
	levelTextController(nullptr),
	inputAllowed(false)
{
	levelSources = FileSystemUtils::GetFilesInFolder(AssetPaths::Levels::SINGLEPLAYER_LEVELS_FOLDER_PATH);
}

void GameManager::Init()
{
	Tools::Helpers::GuardAgainstNull(gameMusicSource, "gameMusicSource was not assigned.");

	inputAllowed = false;

	// Get Map Parser
	auto gameMapParser = SceneManager::FindObjectOfType<GameMapParser>();
	
	Tools::Helpers::GuardAgainstNull(gameMapParser, "GameManager::Init: GameMapParser not found in the scene.");

	// Get Player Position
	auto groupObject = gameMapParser->RetrieveObjectGroup(TiledObjectGroupNames::PLAYER_POSITION_GROUP);

	if (!groupObject.has_value())
	{
		throw std::runtime_error("GameManager::Init: Player position group not found.");
	}

	player = OwningObject.lock()->GetComponentInChildren<Cannon>();
	player->GetGameObject().lock()->GetComponent<Transform>()->SetWorldPosition(groupObject.value().points.front());

	// Generate first level
	tileSize = gameMapParser->GetTileSize();
	auto placements = gameMapParser->RetrieveObjectGroup(TiledObjectGroupNames::GRID_START_GROUP);

	if (!placements.has_value())
	{
		throw std::runtime_error("GameManager::Init: Grid start object group not found.");
	}

	bobbleGrid = OwningObject.lock()->GetComponentInChildren<BobbleGrid>();
	bobbleGrid->GetGameObject().lock()->GetComponent<Transform>()->SetWorldPosition(placements.value().points.front());

	levelTextController = SceneManager::FindObjectOfType<LevelTextController>();

	gridCeiling = OwningObject.lock()->GetComponentInChildren<GridCeiling>(true);

	menuManager = SceneManager::FindObjectOfType<MenuManager>();

	scoreCounter.Init();
}

void GameManager::Start()
{
	CoroutineScheduler::StartCoroutine(StartNextLevel(false));
}

void GameManager::Update(float deltaTime)
{
	if (!inputAllowed)
		return;

	scoreCounter.Update(deltaTime);
}

void GameManager::Draw()
{
}

bool GameManager::IsInputAllowed()
{
	return inputAllowed;
}

void GameManager::SetScoreTextMesh(TextMesh* textMesh)
{
	scoreCounter.SetTextMesh(textMesh);
}

void GameManager::SetGameMusicSource(AudioSource* gameMusicSource)
{
	this->gameMusicSource = gameMusicSource;
}

void GameManager::AddScore(int scoreToAdd)
{
	scoreCounter.AddScore(scoreToAdd);
}

Task GameManager::StartNextLevel(bool displayEndLevelText)
{
	inputAllowed = false;

	currentLevelIndex++;

	auto levelSize = levelSources.size();

	if (currentLevelIndex >= levelSources.size())
	{
		// TODO: Show scoreboard
		gameMusicSource->LoadAudio(AssetPaths::Audio::VICTORY_MUSIC, true);

		gameMusicSource->Play();

		menuManager->ShowMenu(MenuID::VICTORY_MENU);

		co_return;
	};

	if (displayEndLevelText)
	{
		auto completionTime = scoreCounter.GetCurrentTime();

		auto bonusText = "\n" + std::to_string(static_cast<int>(completionTime)) + "sec\n";

		if (completionTime <= BobbleConstants::MIN_TIME_FOR_BONUS_IN_SECONDS)
		{
			auto t = completionTime / BobbleConstants::MIN_TIME_FOR_BONUS_IN_SECONDS;
			auto timeBonus = static_cast<int>(std::lerp(BobbleConstants::MAX_TIME_BONUS, 0, t));
		
			bonusText += "Time Bonus: +" + std::to_string(timeBonus);

			scoreCounter.AddScore(timeBonus);
		}
		else
		{
			bonusText += "No Bonus";
		}

		co_await CoroutineScheduler::StartCoroutine(levelTextController->DisplayEndLevelText("Level " + std::to_string(currentLevelIndex) + " Completed!\n" + bonusText));
	}

	player->GetGameObject().lock()->GetTransform()->SetWorldRotation(0.0f);

	bobbleGrid->SetNewGridLevel(levelSources[currentLevelIndex], tileSize);

	player->ResetLoadedBobble();

	gridCeiling->ResetCeiling();

	co_await CoroutineScheduler::StartCoroutine(levelTextController->DisplayLevelText("Level " + std::to_string(currentLevelIndex)));

	inputAllowed = true;

	scoreCounter.ResetTimer();
}

void GameManager::ShowGameOverScreen()
{
	gameMusicSource->LoadAudio(AssetPaths::Audio::GAMEOVER_MUSIC);

	gameMusicSource->Play();

	menuManager->ShowMenu(MenuID::GAME_OVER_MENU);
}