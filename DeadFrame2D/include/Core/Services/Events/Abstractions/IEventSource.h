#pragma once
#include "Data/Systems/Events/SystemEvent.h"
#include "DF2D_API.h"
#include <optional>


namespace DF2D::Core
{
	class DF2D_API IEventSource
	{
	public:
		virtual ~IEventSource() = default;


		virtual std::optional<Data::SystemEvent> PollEvent() = 0;

		virtual void PushQuitEvent() = 0;
	};
}