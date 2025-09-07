#include "Core/CoreEvents/EventManager.h"
#include "Core/CoreEvents/Abstractions/IEventProcessor.h"


IEventProcessor::IEventProcessor()
{
	EventManager::AddEventProcessor(this);
}

IEventProcessor::~IEventProcessor()
{
	EventManager::RemoveEventProcessor(this);
}
