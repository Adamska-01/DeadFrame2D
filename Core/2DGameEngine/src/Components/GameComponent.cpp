#include "Components/GameComponent.h"
#include "GameObject.h"


GameComponent::GameComponent()
{
	isActive = true;
	isDirty = false;
}

void GameComponent::MarkDirty()
{
	isDirty = true;
}

std::weak_ptr<GameObject> GameComponent::GetGameObject() const
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