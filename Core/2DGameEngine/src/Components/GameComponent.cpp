#include "Components/GameComponent.h"


GameComponent::GameComponent()
{
	OwningObject = nullptr;
	isActive = true;
}

GameObject* GameComponent::GetGameObject() const
{
	return OwningObject;
}

bool GameComponent::IsActive() const
{
	return isActive;
}

void GameComponent::SetActive(bool value)
{
	isActive = value;
}