#include "Components/GameComponent.h"


GameComponent::GameComponent()
{
	OwningObject = nullptr;
	enableDraw = true;
}

GameObject* GameComponent::GetGameObject() const
{
	return OwningObject;
}

bool GameComponent::IsDrawEnabled() const
{
	return enableDraw;
}

void GameComponent::SetEnableDraw(bool value)
{
	enableDraw = value;
}