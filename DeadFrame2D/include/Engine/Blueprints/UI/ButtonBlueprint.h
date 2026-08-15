#pragma once
#include "Data/Blueprints/UI/Button/ButtonBlueprintModel.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"


namespace DF2D::Engine
{
	class DF2D_API ButtonBlueprint : public GameObject
	{
	private:
		Data::ButtonBlueprintModel buttonBlueprintConfiguration;


	public:
		ButtonBlueprint(const Data::ButtonBlueprintModel& buttonBlueprintConfiguration);

		virtual ~ButtonBlueprint() override = default;
	};
}