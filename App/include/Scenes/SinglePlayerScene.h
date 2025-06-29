#pragma once
#include "Scenes/Abstractions/BaseGameScene.h"


class MenuBase;


class SinglePlayerScene : public BaseGameScene
{
private:
	MenuBase* CreateEndScreen(std::string menuTitle, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	MenuBase* CreateHUD();


public:
	SinglePlayerScene() = default;

	virtual ~SinglePlayerScene() override = default;


	virtual void Enter() override;
};