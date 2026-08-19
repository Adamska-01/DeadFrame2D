#pragma once
#include "Core/Services/Time/Abstractions/IClock.h"
#include "Factories/Abstractions/IFactory.h"
#include <memory>


namespace DF2D::Factories
{
	class ClockFactory : public IFactory<std::unique_ptr<Core::IClock>>
	{
	public:
		std::unique_ptr<Core::IClock> CreateProduct() override;
	};
}