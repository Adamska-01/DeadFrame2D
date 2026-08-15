#include "Core/Services/Events/EventManager.h"
#include "Core/Services/Messaging/SystemDialogBox.h"
#include "Core/Services/ServiceInitializer.h"
#include "Core/Services/Time/FrameTimer.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/ECS/System/Scene/SceneManager.h"
#include "Factories/Concretions/Services/Events/EventSourceFactory.h"
#include "Factories/Concretions/Services/Messaging/DialogBackendFactory.h"
#include "Factories/Concretions/Services/Time/ClockFactory.h"
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
			.frameTimer = new FrameTimer(config.rendering.targetFramerate, Factories::ClockFactory().CreateProduct()),
			.systemDialogBox = new SystemDialogBox(Factories::DialogBackendFactory().CreateProduct()),
			.sceneManager = new SceneManager(),
			.eventDispatcher = new EventDispatcher()
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