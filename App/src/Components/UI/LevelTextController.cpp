#include "Components/GameFlow/GameManager.h"
#include "Components/Transform.h"
#include "Components/UI/LevelTextController.h"
#include "Components/UI/TextMesh.h"
#include "GameObject.h"
#include "Tools/Helpers/Coroutines/CoroutineHelpers.h"
#include <Management/SceneManager.h>
#include <SubSystems/Renderer.h>
#include <Tools/Helpers/Guards.h>


LevelTextController::LevelTextController()
	: textMesh(nullptr),
	gameManager(nullptr)
{
}

void LevelTextController::Init()
{
	textMesh = OwningObject.lock()->GetComponent<TextMesh>();
	gameManager = SceneManager::FindObjectOfType<GameManager>();

	Tools::Helpers::GuardAgainstNull(textMesh, "StartLevelText component needs a TextMesh component to function.");
	Tools::Helpers::GuardAgainstNull(gameManager, "StartLevelText component needs a GameManager component to function.");
}

void LevelTextController::Start()
{

}

void LevelTextController::Update(float deltaTime)
{

}

void LevelTextController::Draw()
{

}

Task LevelTextController::DisplayLevelText(std::string text)
{
	OwningObject.lock()->SetActive(true);

	textMesh->SetIsCentered(true);

	textMesh->SetText(text + "\n3");

	co_await Tools::Helpers::Coroutines::WaitSeconds(0.75f);

	textMesh->SetText(text + "\n2");

	co_await Tools::Helpers::Coroutines::WaitSeconds(0.75f);

	textMesh->SetText(text + "\n1");
	
	co_await Tools::Helpers::Coroutines::WaitSeconds(0.75f);

	textMesh->SetText(text + "\nGO!");

	co_await Tools::Helpers::Coroutines::WaitSeconds(0.5f);

	OwningObject.lock()->SetActive(false);
}

Task LevelTextController::DisplayEndLevelText(std::string text)
{
	auto currentPos = OwningObject.lock()->GetTransform()->GetWorldPosition();
	auto renderTargetSize = Renderer::GetResolutionTarget();

	OwningObject.lock()->GetTransform()->SetWorldPosition(Vector2F(renderTargetSize.x * 0.5f, renderTargetSize.y * 0.5f));

	co_await Tools::Helpers::Coroutines::WaitSeconds(1.0f);

	OwningObject.lock()->SetActive(true);

	textMesh->SetIsCentered(true);

	textMesh->SetText(text);

	co_await Tools::Helpers::Coroutines::WaitSeconds(2.0f);

	OwningObject.lock()->SetActive(false);

	OwningObject.lock()->GetTransform()->SetWorldPosition(currentPos);
}