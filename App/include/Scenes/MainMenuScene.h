#pragma once
#include "Scenes/Abstractions/BaseGameScene.h"
#include <Management/Scene.h>
#include <Math/Vector2.h>
#include <memory>
#include <string_view>


class ButtonBlueprint;


class MainMenuScene : public BaseGameScene
{
public:
	MainMenuScene() = default;

	virtual ~MainMenuScene() override = default;


	virtual void Enter() override;
};