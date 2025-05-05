#include "Blueprints/UI/ButtonBlueprint.h"
#include "Components/Transform.h"
#include "Components/UI/Button.h"
#include "Components/UI/TextMesh.h"
#include "Models/Blueprints/UI/ButtonBlueprintModel.h"


ButtonBlueprint::ButtonBlueprint(const ButtonBlueprintModel& buttonBlueprintConfiguration)
{
	AddComponent<Button>(buttonBlueprintConfiguration.buttonComponentModel);

	childObjectCreation = [=]() 
		{
			auto textMeshObject = GameObject::Instantiate<GameObject>();

			auto comp = textMeshObject.lock()->AddComponent<TextMesh>(buttonBlueprintConfiguration.textMeshComponentModel);

			AddChildGameObject(textMeshObject);

			textMeshObject.lock()->GetComponent<Transform>()->SetLocalPosition(Vector2F::Zero);
		};
}

void ButtonBlueprint::Init()
{
	GameObject::Init();

	childObjectCreation();
}