#include "Engine/Blueprints/UI/ButtonBlueprint.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/UI/Button.h"
#include "Engine/Components/UI/TextMesh.h"


ButtonBlueprint::ButtonBlueprint(const ButtonBlueprintModel& buttonBlueprintConfiguration)
	: buttonBlueprintConfiguration(buttonBlueprintConfiguration)
{
	
}

void ButtonBlueprint::ConstructGameObject()
{
	AddComponent<Button>(buttonBlueprintConfiguration.buttonComponentModel);

	auto textMeshObject = GameObject::Instantiate<GameObject>();

	auto comp = textMeshObject.lock()->AddComponent<TextMesh>(buttonBlueprintConfiguration.textMeshComponentModel);

	AddChildGameObject(textMeshObject);

	textMeshObject.lock()->GetComponent<Transform>()->SetLocalPosition(Vector2F::Zero);
}