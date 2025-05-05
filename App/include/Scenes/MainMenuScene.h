#pragma once
#include <Management/Scene.h>
#include <memory>


class ButtonBlueprint;


class MainMenuScene : public Scene
{
private:
	std::weak_ptr<ButtonBlueprint> CreateButton(const std::string& text, const std::function<void()>& onPressedHandler);


public:
	MainMenuScene() = default;

	virtual ~MainMenuScene() override = default;


	virtual void Enter() override;
};