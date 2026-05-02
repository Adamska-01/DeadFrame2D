#include "Core/Services/Events/Abstractions/IEventProcessor.h"
#include "Core/Services/Events/EventManager.h"


namespace DF2D::Core
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