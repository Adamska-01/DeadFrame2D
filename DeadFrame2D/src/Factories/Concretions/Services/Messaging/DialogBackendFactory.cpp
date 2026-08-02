#include "Factories/Concretions/Services/Messaging/DialogBackendFactory.h"
#include "Factories/Products/Services/Messaging/SDLDialogBackend.h"


namespace DF2D::Factories
{
	using namespace DF2D::Internal;


	std::unique_ptr<Core::IDialogBackend> DialogBackendFactory::CreateProduct()
	{
		return std::make_unique<SDLDialogBackend>();
	}
}