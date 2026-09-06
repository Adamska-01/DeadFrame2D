#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"


namespace DF2D::Engine
{
	GameComponent::GameComponent()
	{
		isActive = true;
		isDirty = false;
	}

	GameComponent::~GameComponent() = default;

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

	bool GameComponent::IsActive() const
	{
		if (!isActive)
			return false;

		auto owner = GetGameObject();

		if (owner == nullptr)
			return true;

		return owner->IsActive();
	}

	void GameComponent::SetActive(bool value)
	{
		isActive = value;
	}

	bool GameComponent::IsA(const DF2D::Core::TypeInfo* type) const
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