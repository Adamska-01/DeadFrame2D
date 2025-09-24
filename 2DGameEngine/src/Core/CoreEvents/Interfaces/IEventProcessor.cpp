#include "Core/CoreEvents/EventManager.h"
#include "Core/CoreEvents/Abstractions/IEventProcessor.h"


namespace DeadFrame2D::Core
{
	IEventProcessor::IEventProcessor()
	{
		EventManager::AddEventProcessor(this);
	}

	IEventProcessor::~IEventProcessor()
	{
		EventManager::RemoveEventProcessor(this);
	}
}