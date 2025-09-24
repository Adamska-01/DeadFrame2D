#pragma once
#include "Engine/Entity/GameObject.h"
#include "Data/Blueprints/UI/ButtonBlueprintModel.h"


namespace DeadFrame2D::Engine
{
	class ButtonBlueprint : public GameObject
	{
	private:
		DeadFrame2D::Data::ButtonBlueprintModel buttonBlueprintConfiguration;


	public:
		ButtonBlueprint(const DeadFrame2D::Data::ButtonBlueprintModel& buttonBlueprintConfiguration);

		virtual ~ButtonBlueprint() override = default;


		virtual void ConstructGameObject() override;
	};
}