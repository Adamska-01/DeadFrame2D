#include "Core/Services/Events/EventManager.h"
#include "Core/Services/ServiceInitializer.h"
#include "Core/Services/Time/FrameTimer.h"
#include "Engine/ECS/System/Scene/SceneManager.h"
#include "Factories/Concretions/Services/Events/EventSourceFactory.h"
#include "Helpers/Services/ServiceContextIterator.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Models;
	using namespace DF2D::Internal;


	ServiceInitializer::ServiceInitializer(SystemConfig config)
	{
		ctx = ServiceContext
		{
			.eventManager = new EventManager(Factories::EventSourceFactory().CreateProduct()),
			.frameTimer = new FrameTimer(config.rendering.targetFramerate),
			.sceneManager = new SceneManager()
		};
	}

	ServiceInitializer::~ServiceInitializer()
	{
		ServiceContextIterator::ForEach(ctx, [](auto*& system)
			{
				delete system;
				system = nullptr;
			});
	}

	ServiceContext ServiceInitializer::GetServiceContext() const
	{
		return ctx;
	}
}