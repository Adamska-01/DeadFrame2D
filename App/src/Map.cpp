#include "Map.h"
#include <Components/Collisions/BoxCollider2D.h>
#include <Constants/ScreenConstants.h>
#include <SubSystems/TextureManager.h>
#include <SubSystems/UIManager.h>
#include <UI/UIText.h>
#include <Utility/UsefullFunctions.h>


Map::Map(bool twoPlayers)
{
	// Size of the tile sheet
	srcRect.w = 16;
	srcRect.h = 16;

	// Size of the tile
	destRect.w = 32;
	destRect.h = 32;
	
	// Is it multiplayer?
	howManyPlayers = twoPlayers ? 2 : 1;

	// Sets background image
	background = new Background("Assets/Backgrounds/Stars.png", Vector2(0, ScreenConstants::SCREENHEIGHT), true);
	wallTexture = TextureManager::GetInstance()->LoadTexture("Assets/Sprites/MapTiles.png");
};

Map::~Map()
{
	Clean();
}

void Map::LoadMap(std::string filePath)
{
	//Loads map from file
	UsefullFunctions::LoadMapFromFile(filePath, gridMap);

	//width and height of the vector map
	SetWidth();
	SetHeight();

	//Cannon start position
	SetPlayersPos();
}

void Map::LoadColliders(std::string filePath)
{
	UsefullFunctions::LoadMapFromFile(filePath, gridColliders);
	
	AddColliders(); //Add colliders from the loaded map
}

void Map::AddColliders()
{
	auto startX1 = 0;
	auto startX2 = 0;

	for (auto row = 0; row < height; row++)
	{
		for (auto column = 0; column < width; column++)
		{
			destRect.x = column * destRect.w;
			destRect.y = row * destRect.h;

			auto length = 0;

			switch (gridColliders[row][column])
			{
			case 'V':
				walls[Walls::P1_WALL].push_back(BoxCollider2D());
				length = UsefullFunctions::CountCharactersinAColumn(gridColliders, row, column);
				walls[Walls::P1_WALL].back().SetBox(destRect.x, destRect.y, destRect.w, destRect.h * length);
				break;

			case 'v':
				walls[Walls::P2_WALL].push_back(BoxCollider2D());
				length = UsefullFunctions::CountCharactersinAColumn(gridColliders, row, column);
				walls[Walls::P2_WALL].back().SetBox(destRect.x, destRect.y, destRect.w, destRect.h * length);
				break;

			case 'G':
				walls[Walls::P1_GAMEOVER].push_back(BoxCollider2D());
				length = UsefullFunctions::CountCharactersinARow(gridColliders, row, column) + 1;
				walls[Walls::P1_GAMEOVER].back().SetBox(destRect.x, destRect.y, destRect.w * length, destRect.h);
				//Spawner
				startX1 = destRect.x + (destRect.w * (length / 2));
				break;

			case 'g':
				walls[Walls::P2_GAMEOVER].push_back(BoxCollider2D());
				length = UsefullFunctions::CountCharactersinARow(gridColliders, row, column) + 1;
				walls[Walls::P2_GAMEOVER].back().SetBox(destRect.x, destRect.y, destRect.w * length, destRect.h);
				//Spawner
				startX2 = destRect.x + (destRect.w * (length / 2));
				break;

			default:
				break;
			}
		}
	}

	//instantiate spawner
	for (auto i = 0; i < howManyPlayers; i++)
	{
		Spawner.push_back(new BobbleSpawner((i == 0) ? startX1 : startX2));
	}
}

void Map::LoadUI(std::string filePath)
{
	UsefullFunctions::LoadMapFromFile(filePath, gridUI);

	AddUI();  //Add UI from the loaded map
}

void Map::AddUI()
{
	for (auto row = 0; row < height; row++)
	{
		for (auto column = 0; column < width; column++)
		{
			destRect.x = column * destRect.w;
			destRect.y = row * destRect.h;
			
			// TODO: do some shit with the screen size to determine the position.
			switch (gridUI[row][column])
			{
			case 'S': 
			{
				auto ui = new UIText(Fonts::BOBBLE, " ");
				ui->ChangeColor(0, 55, 205, 255);
				ui->SetPos(destRect.x, destRect.y);

				UI.push_back(ui);
				break;
			}

			case 's':
			{
				auto ui = new UIText(Fonts::BOBBLE, " ");
				ui->ChangeColor(243, 38, 15, 255);
				ui->SetPos(destRect.x, destRect.y);

				UI.push_back(ui);
				break;
			}

			default:
				break;
			}
		}
	}
}

void Map::Update(float dt)
{
	// Spawners update
	for (auto spawn : Spawner)
	{
		spawn->Update(dt);
	}

	// Handle wall collisions for all players
	for (auto playerIndex = 0; playerIndex < howManyPlayers; ++playerIndex)
	{
		// For each player, check their wall and game over collisions
		auto wallKey = (playerIndex == 0) ? Walls::P1_WALL : Walls::P2_WALL;
		auto gameOverKey = (playerIndex == 0) ? Walls::P1_GAMEOVER : Walls::P2_GAMEOVER;

		for (BoxCollider2D box : GetWallByKey(wallKey))
		{
			Spawner[playerIndex]->CollisionsWall(box.GetBox());
		}

		// Handle game over collisions
		for (BoxCollider2D box : GetWallByKey(gameOverKey))
		{
			Spawner[playerIndex]->CollisionsWall(box.GetBox(), true);
		}
	}

	//Background
	background->Update(dt);
	
	//UI
	for (auto i = 0; i < UI.size(); i++)
	{
		//Create text
		std::stringstream score;
		score << "Score:\n" << Spawner[i]->GetStats()->GetScore() << "\nLife:\n" << Spawner[i]->GetStats()->GetLives();
		std::string Score = score.str();
		
		UI[i]->UpdateText(Score, 2);
	}
}

void Map::Draw()
{
	//Background
	background->Draw();
	
	for (auto row = 0; row < height; row++)
	{
		for (auto column = 0; column < width; column++)
		{
			destRect.x = column * destRect.w;
			destRect.y = row * destRect.h;

			switch (gridMap[row][column])
			{
			case '4':
				srcRect.x = srcRect.w * 3;
				srcRect.y = srcRect.h * 2;
				TextureManager::GetInstance()->DrawTile(wallTexture, srcRect, destRect, row, column);
				break;

			case 'a':
				srcRect.x = srcRect.w * 3;
				srcRect.y = srcRect.h * 2;
				TextureManager::GetInstance()->DrawTile(wallTexture, srcRect, destRect, row, column);
				break;

			case '5':
				srcRect.x = srcRect.w * 5;
				srcRect.y = srcRect.h * 2;
				TextureManager::GetInstance()->DrawTile(wallTexture, srcRect, destRect, row, column);
				break;

			case '6':
				srcRect.x = srcRect.w * 2;
				srcRect.y = srcRect.h * 3;
				TextureManager::GetInstance()->DrawTile(wallTexture, srcRect, destRect, row, column);
				break;

			case 'P':
				srcRect.x = srcRect.w * 2;
				srcRect.y = srcRect.h * 3;
				TextureManager::GetInstance()->DrawTile(wallTexture, srcRect, destRect, row, column);
				break;

			case 'p':
				srcRect.x = srcRect.w * 2;
				srcRect.y = srcRect.h * 3;
				TextureManager::GetInstance()->DrawTile(wallTexture, srcRect, destRect, row, column);
				break;

			case '7':
				srcRect.x = srcRect.w * 3;
				srcRect.y = srcRect.h * 3;
				TextureManager::GetInstance()->DrawTile(wallTexture, srcRect, destRect, row, column);
				break;

			case '8':
				srcRect.x = srcRect.w * 4;
				srcRect.y = srcRect.h * 3;
				TextureManager::GetInstance()->DrawTile(wallTexture, srcRect, destRect, row, column);
				break;

			case '9':
				srcRect.x = srcRect.w * 5;
				srcRect.y = srcRect.h * 3;
				TextureManager::GetInstance()->DrawTile(wallTexture, srcRect, destRect, row, column);
				break;

			default:
				break;
			}
		}
	}

	//Draw box colliders
#if _DEBUG
	for (const auto& pair : walls)
	{
		for (BoxCollider2D d : pair.second)
		{
			d.DrawBox(255,255,255,255);
		}
	}
#endif

	//UI
	for (auto i = 0; i < UI.size(); i++)
	{
		UI[i]->Draw(Vector2::One);
	}

	//Spawner(s)
	for (auto it : Spawner)
	{
		it->Draw();
	}
}

void Map::Clean()
{
	if (background != nullptr)
	{
		delete background;
		background = nullptr;
	}

	if (Spawner.size() > 0)
	{
		for (int i = 0; i < Spawner.size(); i++)
		{
			delete Spawner[i];
			Spawner[i] = nullptr;
		}

		Spawner.clear();
	}

	if (UI.size() > 0)
	{
		for (int i = 0; i < UI.size(); i++)
		{
			delete UI[i];
			UI[i] = nullptr;
		}

		UI.clear();
	}
}

void Map::SetPlayersPos()
{
	for (auto row = 0; row < height; row++)
	{
		for (auto column = 0; column < width; column++)
		{
			destRect.x = column * destRect.w;
			destRect.y = row * destRect.h;
			
			switch (gridMap[row][column])
			{
			case 'P': //Player 1
				P1Pos.x = destRect.x - (destRect.w / 4.0f);
				P1Pos.y = destRect.y - (destRect.h / 2.0f);
				break;

			case 'p': //Player 2
				P2Pos.x = destRect.x - (destRect.w / 4.0f);
				P2Pos.y = destRect.y - (destRect.h / 2.0f);
				break;
			
			default:
				break;
			}
		}
	}
}

void Map::SetWidth()
{
	width = gridMap[0].length();
}

void Map::SetHeight()
{
	height = gridMap.size();
}

Vector2 Map::GetP1Pos() const
{
	return P1Pos;
}

Vector2 Map::GetP2Pos() const
{
	return P2Pos;
}

const std::map<Walls, std::vector<BoxCollider2D>>& Map::GetWalls()
{ 
	return walls; 
}

const std::vector<BoxCollider2D>& Map::GetWallByKey(Walls key)
{ 
	return walls[key]; 
}

const std::vector<BobbleSpawner*>& Map::GetSpawner()
{ 
	return Spawner; 
}