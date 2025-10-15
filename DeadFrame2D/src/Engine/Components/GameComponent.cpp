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

	void GameComponent::Init()
	{

	}

	void GameComponent::Start()
	{

	}

	void GameComponent::Update(float deltaTime)
	{

	}

	void GameComponent::LateUpdate(float deltaTime)
	{

	}

	void GameComponent::Draw()
	{

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

	bool GameComponent::IsA(const DeadFrame2D::Core::TypeInfo* type) const
	{
		const auto* current = GetTypeInfo();

		while (current)
		{
			if (current == type)
				return true;
		
			current = current->parent;
		}
		
		return false;
	}
}