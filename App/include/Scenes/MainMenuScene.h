#pragma once
#include <Management/Scene.h>


class MainMenuScene : public Scene
{
public:
	MainMenuScene() = default;

	virtual ~MainMenuScene() override = default;


	virtual void Enter() override;
};