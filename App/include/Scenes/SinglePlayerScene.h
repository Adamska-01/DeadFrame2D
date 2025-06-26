#pragma once
#include <Blueprints/UI/ButtonBlueprint.h>
#include <Management/Scene.h>
#include <memory>


class MenuBase;


class SinglePlayerScene : public Scene
{
private:
	std::weak_ptr<GameObject> CreateText(const std::string& text);

	std::weak_ptr<ButtonBlueprint> CreateButton(const std::string& text, const std::function<void()>& onPressedHandler);

	MenuBase* CreateEndScreen(std::string menuTitle, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	MenuBase* CreateHUD();


public:
	SinglePlayerScene() = default;

	virtual ~SinglePlayerScene() override = default;


	virtual void Enter() override;
};