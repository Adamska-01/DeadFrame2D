#include "Blueprints/UI/ButtonBlueprint.h"
#include "Components/Transform.h"
#include "Components/UI/Button.h"
#include "Components/UI/TextMesh.h"


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