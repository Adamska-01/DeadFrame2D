#include "Engine/Blueprints/UI/ButtonBlueprint.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Button.h"
#include "Engine/ECS/Component/UI/TextMesh.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	ButtonBlueprint::ButtonBlueprint(const ButtonBlueprintModel& buttonBlueprintConfiguration)
		: buttonBlueprintConfiguration(buttonBlueprintConfiguration)
	{
	
	}

	void ButtonBlueprint::ConstructGameObject()
	{
		AddComponent<Button>(buttonBlueprintConfiguration.buttonComponentModel);

		auto textMeshObject = GameObject::Instantiate<GameObject>();

		auto comp = textMeshObject->AddComponent<TextMesh>(buttonBlueprintConfiguration.textMeshComponentModel);

		textMeshObject->SetParent(thisGameObject);

		textMeshObject->GetTransform()->SetLocalPosition(Vector2F::Zero);
	}
}