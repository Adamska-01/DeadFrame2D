#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Engine
{
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
}