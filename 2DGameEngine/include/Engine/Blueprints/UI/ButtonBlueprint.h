#pragma once
#include "Engine/Entity/GameObject.h"
#include "Data/Blueprints/UI/ButtonBlueprintModel.h"


class ButtonBlueprint : public GameObject
{
private:
	ButtonBlueprintModel buttonBlueprintConfiguration;


public:
	ButtonBlueprint(const ButtonBlueprintModel& buttonBlueprintConfiguration);

	virtual ~ButtonBlueprint() override = default;


	virtual void ConstructGameObject() override;
};