#pragma once
#include "Core/Services/Events/Abstractions/IEventSource.h"
#include "Factories/Abstractions/IFactory.h"
#include <memory>


namespace DF2D::Factories
{
	class EventSourceFactory : public IFactory<std::unique_ptr<Core::IEventSource>>
	{
	public:
		std::unique_ptr<Core::IEventSource> CreateProduct() override;
	};
}