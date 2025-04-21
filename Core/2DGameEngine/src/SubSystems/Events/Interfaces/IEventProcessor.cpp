#include "SubSystems/Events/EventManager.h"
#include "SubSystems/Events/Interfaces/IEventProcessor.h"


IEventProcessor::IEventProcessor()
{
	EventManager::AddEventProcessor(this);
}

IEventProcessor::~IEventProcessor()
{
	EventManager::RemoveEventProcessor(this);
}
