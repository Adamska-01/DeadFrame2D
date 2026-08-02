#pragma once
#include "Core/Services/Time/Abstractions/IClock.h"
#include <memory>


namespace DF2D::Factories
{
	class ClockFactory
	{
	public:
		std::unique_ptr<Core::IClock> CreateProduct();
	};
}