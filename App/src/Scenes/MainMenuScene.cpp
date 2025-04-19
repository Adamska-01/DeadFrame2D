#include "Scenes/MainMenuScene.h"
#include <Components/UI/Button.h>
#include <Components/Transform.h>
#include <Subsystems/Renderer.h>
#include <Application.h>
#include <Scenes/GameScene.h>


void MainMenuScene::Enter()
{
	auto a = GameObject::Instantiate<GameObject>();
	a.lock()->AddComponent<Button>(
		[]() { SceneManager::LoadScene(std::make_unique<GameScene>()); },
		"App/Assets/Sprites/Buttons/Button_Idle.png",
		"App/Assets/Sprites/Buttons/Button_Hovered.png",
		"App/Assets/Sprites/Buttons/Button_Pressed.png",
		"Press To Crash",
		Vector2F{ 300.0f, 100.0f }
	);
	a.lock()->GetComponent<Transform>()->position = Vector2F(Window::GetResolution().x * 0.5f, Window::GetResolution().y * 0.5f);
}
