#pragma once
#include "Data/Services/ServiceContext.h"
#include "Data/Systems/CoreContext.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/ECS/System/Scene/Scene.h"


class TestScene : public DF2D::Engine::Scene
{
public:
	explicit TestScene(DF2D::Engine::EventDispatcher* eventDispatcher)
		: DF2D::Engine::Scene(eventDispatcher)
	{
	}

	void Enter() override
	{
	}

	template<typename T, typename... Args>
	DF2D::Engine::ObjectHandle<T> Spawn(Args&&... args)
	{
		return Instantiate<T>(DF2D::Data::CoreContext{}, DF2D::Data::ServiceContext{}, std::forward<Args>(args)...);
	}
};