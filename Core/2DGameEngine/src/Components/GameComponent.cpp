#include "Components/GameComponent.h"
#include "GameObject.h"


GameComponent::GameComponent()
{
	OwningObject = nullptr;
	isActive = true;
	isDirty = true;
}

void GameComponent::MarkDirty()
{
	isDirty = true;
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