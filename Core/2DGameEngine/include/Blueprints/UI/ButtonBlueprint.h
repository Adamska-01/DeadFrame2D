#pragma once
#include "GameObject.h"


struct ButtonBlueprintModel;


class ButtonBlueprint : public GameObject
{
private:
	std::function<void()> childObjectCreation;


public:
	ButtonBlueprint(const ButtonBlueprintModel& buttonBlueprintConfiguration);

	virtual ~ButtonBlueprint() override = default;


	virtual void Init() override;
};