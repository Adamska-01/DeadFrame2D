#include "Scenes/GameScene.h"
#include "Scenes/MainMenuScene.h"
#include <Components/Transform.h>
#include <Components/UI/Button.h>
#include <Management/SceneManager.h>
#include <SubSystems/Window.h>


void MainMenuScene::Enter()
{
	auto a = GameObject::Instantiate<GameObject>();
	a.lock()->AddComponent<Button>(
		[]() { SceneManager::LoadScene<GameScene>(); },
		"App/Assets/Sprites/Buttons/Button_Idle.png",
		"App/Assets/Sprites/Buttons/Button_Hovered.png",
		"App/Assets/Sprites/Buttons/Button_Pressed.png",
		"Press To Crash",
		Vector2F{ 300.0f, 100.0f }
	);
	auto dumb = GameObject::Instantiate<GameObject>();
	auto dumb2 = GameObject::Instantiate<GameObject>();
	dumb.lock()->AddChildGameObject(dumb2);
	a.lock()->AddChildGameObject(dumb);
	a.lock()->AddChildGameObject(dumb2);

	auto components = a.lock()->GetComponentsInChildren<Transform>();
	a.lock()->GetComponent<Transform>()->SetWorldPosition(Vector2F(Window::GetResolution().x * 0.5f, Window::GetResolution().y * 0.5f));
}
