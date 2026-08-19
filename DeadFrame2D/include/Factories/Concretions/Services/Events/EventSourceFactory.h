#pragma once
#include "Core/Services/Events/Abstractions/IEventSource.h"
#include "DF2D_API.h"
#include "Factories/Abstractions/IFactory.h"
#include <memory>


namespace DF2D::Factories
{
	class DF2D_API EventSourceFactory : public IFactory<std::unique_ptr<Core::IEventSource>>
	{
	public:
		std::unique_ptr<Core::IEventSource> CreateProduct() override;
	};
}