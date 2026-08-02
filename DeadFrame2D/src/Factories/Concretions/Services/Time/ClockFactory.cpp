#include "Factories/Concretions/Services/Time/ClockFactory.h"
#include "Factories/Products/Services/Time/SteadyClock.h"


namespace DF2D::Factories
{
	using namespace DF2D::Internal;


	std::unique_ptr<Core::IClock> ClockFactory::CreateProduct()
	{
		return std::make_unique<SteadyClock>();
	}
}