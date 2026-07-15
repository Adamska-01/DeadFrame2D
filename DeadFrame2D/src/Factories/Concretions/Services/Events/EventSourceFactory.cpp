#include "Factories/Concretions/Services/Events/EventSourceFactory.h"
#include "Factories/Products/Services/Events/SDLEventSource.h"


namespace DF2D::Factories
{
	using namespace DF2D::Internal;


	std::unique_ptr<Core::IEventSource> EventSourceFactory::CreateProduct()
	{
		return std::make_unique<SDLEventSource>();
	}
}