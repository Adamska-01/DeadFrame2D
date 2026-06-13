#include "Factories/Concretions/Context/Systems/Window/WindowBackendFactory.h"
#include "Factories/Products/Context/Systems/Window/SDLWindowBackend.h"


namespace DF2D::Factories
{
	using namespace DF2D::Core;
	using namespace DF2D::Models;


	std::unique_ptr<IWindowBackend> WindowBackendFactory::CreateProduct(WindowConfig config)
	{
		return std::make_unique<SDLWindowBackend>(config.title, config.width, config.height, config.fullscreen);
	}
}