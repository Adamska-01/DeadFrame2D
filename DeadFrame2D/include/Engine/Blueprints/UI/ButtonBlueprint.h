#pragma once
#include "Data/Blueprints/UI/ButtonBlueprintModel.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API ButtonBlueprint : public GameObject
	{
	private:
		DeadFrame2D::Data::ButtonBlueprintModel buttonBlueprintConfiguration;


	public:
		ButtonBlueprint(const DeadFrame2D::Data::ButtonBlueprintModel& buttonBlueprintConfiguration);

		virtual ~ButtonBlueprint() override = default;


		virtual void ConstructGameObject() override;
	};
}