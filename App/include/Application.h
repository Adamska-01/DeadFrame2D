#pragma once
#include <Utility/FrameTimer.h>
#include <vector>


class UIText;
class Map;
class Background;
class Cannon;
struct SDL_Texture;


class Application
{
private:
	bool running{ false };

	bool gameover{ false };
	
	bool twoPlayers{ false };

	std::vector<Cannon*> cannon{ nullptr };

	Map* map{ nullptr };

	Background* menuBackground;

	UIText* FPSText;
	
	UIText* gameOverText;

	FrameTimer ft;


	void MenuUpdate();
	
	void Quit();
	
	void Destroy();
	
	SDL_Texture* EndGameFadeTexture();


public:
	Application();

	~Application();


	void Update();
};