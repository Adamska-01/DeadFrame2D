#include "Application.h"
#include "Scenes/MainMenuScene.h"


Application::Application()
{
	SceneManager::LoadScene<MainMenuScene>();
}