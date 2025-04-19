#include "Application.h"
#include "Scenes/MainMenuScene.h"


Application::Application()
{
	SceneManager::LoadScene(std::make_unique<MainMenuScene>());
}