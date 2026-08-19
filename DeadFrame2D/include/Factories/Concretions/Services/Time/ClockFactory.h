#pragma once
#include "Core/Services/Time/Abstractions/IClock.h"
#include "DF2D_API.h"
#include "Factories/Abstractions/IFactory.h"
#include <memory>


namespace DF2D::Factories
{
	class DF2D_API ClockFactory : public IFactory<std::unique_ptr<Core::IClock>>
	{
	public:
		std::unique_ptr<Core::IClock> CreateProduct() override;
	};
}