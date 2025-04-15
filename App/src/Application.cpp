#include "Application.h"
#include <Scenes/GameScene.h>


Application::Application()
{
	SceneManager::LoadScene(std::make_unique<GameScene>());
}