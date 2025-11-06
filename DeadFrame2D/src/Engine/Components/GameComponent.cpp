#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/GameObject.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandle.h"


namespace DeadFrame2D::Engine
{
	struct GameComponent::Impl
	{
		ObjectHandle<GameObject> cachedHandle;
	};


	GameComponent::GameComponent()
		: pImpl(std::make_shared<Impl>())
	{
		isActive = true;
		isDirty = false;
	}

	GameComponent::~GameComponent() = default;

	void GameComponent::SetGameObject(const ObjectHandle<GameObject>& handle)
	{
		gameObject = handle;
		pImpl->cachedHandle = handle;
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

	bool GameComponent::IsActive() const
	{
		return isActive && GetGameObject()->IsActive();
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

	ObjectHandle<GameObject> GameComponent::GetGameObject() const
	{
		return pImpl->cachedHandle;
	}
}