#pragma once
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"


class DummyGameComponent : public DF2D::Engine::GameComponent
{
	TYPE_INFO(DummyGameComponent, DF2D::Engine::GameComponent);


public:
	int initCallCount = 0;

	int updateCallCount = 0;

	float lastDeltaTime = 0.0f;


	void Init() override
	{
		initCallCount++;
	}

	void Update(float deltaTime) override
	{
		updateCallCount++;
		lastDeltaTime = deltaTime;
	}

	bool IsDirty() const
	{
		return isDirty;
	}
};


class OtherDummyGameComponent : public DF2D::Engine::GameComponent
{
	TYPE_INFO(OtherDummyGameComponent, DF2D::Engine::GameComponent);
};


class DerivedDummyGameComponent : public DummyGameComponent
{
	TYPE_INFO(DerivedDummyGameComponent, DummyGameComponent);
};
