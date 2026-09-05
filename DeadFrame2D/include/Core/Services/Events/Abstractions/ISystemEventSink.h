#pragma once
#include "Data/Systems/Events/SystemEvent.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Receives platform events pumped by the event manager.
	 */
	class DF2D_API ISystemEventSink
	{
	public:
		virtual ~ISystemEventSink() = default;


		virtual void OnSystemEvent(const Data::SystemEvent& systemEvent) = 0;
	};
}