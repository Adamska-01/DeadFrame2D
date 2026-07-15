#pragma once
#include "Core/Services/Events/Abstractions/IEventSource.h"
#include <memory>


namespace DF2D::Factories
{
	class EventSourceFactory
	{
	public:
		std::unique_ptr<Core::IEventSource> CreateProduct();
	};
}