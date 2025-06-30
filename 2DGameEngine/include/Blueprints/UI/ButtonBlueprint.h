#pragma once
#include "GameObject.h"
#include "Models/Blueprints/UI/ButtonBlueprintModel.h"


class ButtonBlueprint : public GameObject
{
private:
	ButtonBlueprintModel buttonBlueprintConfiguration;


public:
	ButtonBlueprint(const ButtonBlueprintModel& buttonBlueprintConfiguration);

	virtual ~ButtonBlueprint() override = default;


	virtual void ConstructGameObject() override;
};