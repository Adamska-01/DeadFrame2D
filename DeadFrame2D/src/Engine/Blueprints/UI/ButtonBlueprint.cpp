#include "Engine/Blueprints/UI/ButtonBlueprint.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/UI/Button.h"
#include "Engine/Components/UI/TextMesh.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;


	ButtonBlueprint::ButtonBlueprint(const ButtonBlueprintModel& buttonBlueprintConfiguration)
		: buttonBlueprintConfiguration(buttonBlueprintConfiguration)
	{
	
	}

	void ButtonBlueprint::ConstructGameObject()
	{
		AddComponent<Button>(buttonBlueprintConfiguration.buttonComponentModel);

		auto textMeshObject = GameObject::Instantiate<GameObject>();

		auto comp = textMeshObject->AddComponent<TextMesh>(buttonBlueprintConfiguration.textMeshComponentModel);

		AddChildGameObject(textMeshObject);

		textMeshObject->GetTransform()->SetLocalPosition(Vector2F::Zero);
	}
}