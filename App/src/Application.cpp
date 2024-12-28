#include "Application.h"
#include "Bobble.h"
#include "Cannon.h"
#include "Map.h"
#include <Components/Collisions/BoxCollider2D.h>
#include <Constants/ScreenConstants.h>
#include <Debugging/MemoryManagement.h>
#include <MessageBox/MessageBoxConfig.h>
#include <MessageBox/MessageHandler.h>
#include <sstream>
#include <stdlib.h>
#include <SubSystems/AudioManager.h>
#include <SubSystems/Input.h>
#include <SubSystems/TextureManager.h>
#include <time.h>
#include <UI/UIText.h>


const SDL_MessageBoxButtonData button = 
{ 
	SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 
	0, 
	"OK" 
};

const MessageBoxConfig controllerMessage(
	"Controller Error",
	"Controller not detected !!! - \nInsert controller and press Escape",
	SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR,
	std::vector<SDL_MessageBoxButtonData>{ button });

const MessageBoxConfig instructionMessage(
	"Controller instructions",
	"Shoot the falling bobbles before they reach the ground! (MATCH THE COLOUR!!!) \n\nA = Shoot \n<- and -> = rotate cannon \nLB and RB = Change bobble colour\n\nPress Escape to close this box",
	SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR,
	std::vector<SDL_MessageBoxButtonData>{ button });

const MessageBoxConfig instructionMessag("", "", SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, std::vector< SDL_MessageBoxButtonData>());


Application::Application()
{
	srand(time(NULL));

	running = true;
	gameover = false;
	twoPlayers = false;

	//Load Fonts
	UIManager::GetInstance()->LoadFont(Fonts::BOBBLE, "Assets/Fonts/consola.ttf", 40);

	//Audio SFX
	AudioManager::GetInstance()->LoadAudio(0, "Assets/Audio/Bubble Blow.wav", SFX, 20);
	AudioManager::GetInstance()->LoadAudio(1, "Assets/Audio/Spawn.ogg", SFX, 20);
	AudioManager::GetInstance()->LoadAudio(2, "Assets/Audio/Bounce.wav", SFX, 5);
	AudioManager::GetInstance()->LoadAudio(3, "Assets/Audio/bubble-shot.wav", SFX, 10);
	AudioManager::GetInstance()->LoadAudio(4, "Assets/Audio/GameOver.mp3", SFX, 40);
	
	//Audio Music
	AudioManager::GetInstance()->LoadAudio(0, "Assets/Audio/AIr Room.wav", MUSIC, 15);
	AudioManager::GetInstance()->LoadAudio(1, "Assets/Audio/Blazer Rail 2.wav", MUSIC, 15);

	//Player
	cannon.clear();

	//FPS text
	FPSText = new UIText(Fonts::BOBBLE, " ");
	FPSText->SetPos(0, 0);
	FPSText->ChangeColor(235, 219, 7, 255);

	//GameOver text
	gameOverText = new UIText(Fonts::BOBBLE, " ");
	gameOverText->SetPos(ScreenConstants::SCREENWIDTH / 2, ScreenConstants::SCREENHEIGHT / 2);
	gameOverText->ChangeColor(0, 205, 55, 255);
}

Application::~Application()
{
	Destroy();
}

void Application::Update()
{
	Input::GetInstance()->Update();

	//Check if at least one controller is plugged in
	MessageHandler Message;
	if (Input::GetInstance()->GetControllersNumber() == 0)
	{
		Message.ShowMessageBox(controllerMessage);
	}

	//-------------------MAIN MENU----------------------
	MenuUpdate();

	//----------INIT MAP, COLLIDERS, GAME MODE-------------------
	AudioManager::GetInstance()->FadeMusicTrack(1, -1, 1000);
	map = new Map(twoPlayers);

	//Game instructions
	if (running)
	{
		Message.ShowMessageBox(instructionMessage);
	}

	auto mapPath = twoPlayers ? "Assets/Map/Multiplayer/Map.txt" : "Assets/Map/SinglePlayer/Map.txt";
	auto colliderPath = twoPlayers ? "Assets/Map/Multiplayer/BoxColliders.txt" : "Assets/Map/SinglePlayer/BoxColliders.txt";
	auto uiPath = twoPlayers ? "Assets/Map/Multiplayer/UIText.txt" : "Assets/Map/SinglePlayer/UIText.txt";

	map->LoadMap(mapPath);

	cannon.push_back(new Cannon(map->GetP1Pos(), (int)Players::PLAYER1));
	if (twoPlayers) 
	{
		cannon.push_back(new Cannon(map->GetP2Pos(), (int)Players::PLAYER2));
	}

	map->LoadColliders(colliderPath);
	map->LoadUI(uiPath);

	auto transparentBackground = EndGameFadeTexture();

	//---------------GAME UPDATE------------------ 
	while (running)
	{
		if (Renderer::GetInstance()->GetRenderer())
		{
			ft.StartClock();

			if (!Input::GetInstance()->Update())
				Quit();

			if (!gameover)
			{
				//Updates
				for (auto it : cannon)
				{
					it->Update(FrameTimer::DeltaTime());
				}

				map->Update(FrameTimer::DeltaTime());

				//Check if gameover
				for (int i = 0; i < map->GetSpawner().size(); i++)
				{
					if (map->GetSpawner()[i]->GetStats()->GetLives() <= 0)
					{
						map->GetSpawner()[i]->RemoveTimers();

						cannon[i]->SetGameover(true);
					}
				}

				if ((twoPlayers && cannon.front()->GetGameOver() && cannon.back()->GetGameOver()) 
					|| (!twoPlayers && cannon.front()->GetGameOver()))
				{
					gameover = true;

					AudioManager::GetInstance()->StopMusic();
					AudioManager::GetInstance()->PlaySFX(4, 0, 0);
				}


				//Collisions (Player's bobble) with wall and falling bobbles
				auto index = 0;
				for (auto it : cannon)
				{
					if (it->GetBobble() == nullptr)
						continue;

					if (index == 0)
					{
						for (BoxCollider2D i : map->GetWallByKey(Walls::P1_WALL))
						{
							it->GetBobble()->CollisionsWithWall(i.GetBox());
						}
						map->GetSpawner().front()->CollisionsBobble(it->GetBobble());
					}
					else
					{
						for (BoxCollider2D i : map->GetWallByKey(Walls::P2_WALL))
						{
							it->GetBobble()->CollisionsWithWall(i.GetBox());
						}
						map->GetSpawner().back()->CollisionsBobble(it->GetBobble());
					}

					++index;
				}
			}

			//Rendering
			map->Draw();

			for (auto it : cannon)
			{
				it->Draw();
			}

			//End game
			if (gameover)
			{
				//Create text
				std::stringstream message;

				if (twoPlayers)
				{
					if (map->GetSpawner().front()->GetStats()->GetScore() > map->GetSpawner().back()->GetStats()->GetScore())
					{
						message << "The winner is Player 1!\nScore: " << map->GetSpawner().front()->GetStats()->GetScore() << "\n\nPress B to exit the game...";
					}
					else if (map->GetSpawner().front()->GetStats()->GetScore() < map->GetSpawner().back()->GetStats()->GetScore())
					{
						message << "The winner is Player 2!\nScore: " << map->GetSpawner().back()->GetStats()->GetScore() << "\n\nPress B to exit the game...";
					}
					else
					{
						message << "It's a draw!\n" << "Press B to exit the game...";
					}
				}
				else
				{
					message << "GameOver\n" << "Your Score: " << map->GetSpawner().front()->GetStats()->GetScore() << "\n\nPress B to exit the game...";
				}

				TextureManager::GetInstance()->NormalDraw(transparentBackground);

				gameOverText->UpdateText(message.str(), 2);
				gameOverText->Draw(Vector2::One, true);

				if (Input::GetInstance()->ButtonPressed(Players::PLAYER1, SDL_CONTROLLER_BUTTON_B))
				{
					Quit();
				}
			}

			Renderer::GetInstance()->ClearAndPresent();

			//FPS and delay
			ft.EndClock();
			ft.DelayByFrameTime();

			FPSText->UpdateText(std::to_string(FrameTimer::Frames()), 2);

			FPSText->Draw(Vector2::One);
		}
	}
}

void Application::MenuUpdate()
{
	AudioManager::GetInstance()->PlayMusicTrack(0, -1);

	menuBackground = new Background("Assets/Backgrounds/Stars.png", Vector2(0, 720), true);

	auto IsMenu = true;
	auto frameTime = (double)1000 / 60;

	auto menuImage = TextureManager::GetInstance()->LoadTexture("Assets/Menu.png");

	while (IsMenu)
	{
		ft.StartClock();

		menuBackground->Update(FrameTimer::DeltaTime());
		menuBackground->Draw();

		TextureManager::GetInstance()->Draw(menuImage, SDL_Rect{ 0, 0, ScreenConstants::SCREENWIDTH, ScreenConstants::SCREENHEIGHT }, Vector2::One);

		Renderer::GetInstance()->ClearAndPresent();

		if (!Input::GetInstance()->Update())
		{
			IsMenu = false;
			Quit();
		}
		if (Input::GetInstance()->ButtonPressed(PLAYER1, SDL_CONTROLLER_BUTTON_A))
		{
			twoPlayers = false;
			IsMenu = false;
		}
		else if (Input::GetInstance()->ButtonPressed(PLAYER1, SDL_CONTROLLER_BUTTON_X))
		{
			twoPlayers = true;
			IsMenu = false;
		}

		ft.EndClock();
		ft.DelayByFrameTime();

		FPSText->UpdateText(std::to_string(FrameTimer::Frames()), 2);
		FPSText->Draw(Vector2::One);
	}
}

//Creates a fading texture
SDL_Texture* Application::EndGameFadeTexture()
{
	auto tempSurface = SDL_CreateRGBSurface(0, ScreenConstants::SCREENWIDTH, ScreenConstants::SCREENHEIGHT, 32, 0, 0, 0, 0);

	auto colour = SDL_MapRGBA(tempSurface->format, 0, 0, 0, 0);

	SDL_FillRect(tempSurface, NULL, colour);

	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetRenderer(), tempSurface);

	SDL_FreeSurface(tempSurface);
	tempSurface = nullptr;

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(texture, 150);

	return texture;
}

void Application::Destroy()
{
	//Singletons
	TextureManager::GetInstance()->Clean();
	UIManager::GetInstance()->Clean();
	AudioManager::GetInstance()->Clean();
	Input::GetInstance()->Clean();
	Renderer::GetInstance()->Destroy();
	Input::GetInstance()->Clean();

	if (menuBackground != nullptr)
	{
		delete menuBackground;
		menuBackground = nullptr;
	}

	if (cannon.size() > 0)
	{
		for (int i = 0; i < cannon.size(); i++)
		{
			delete cannon[i];
			cannon[i] = nullptr;
		}
		cannon.clear();
	}

#if _DEBUG
	file.close();
#endif

	SDL_Quit();
}

void Application::Quit()
{
	running = false;
}