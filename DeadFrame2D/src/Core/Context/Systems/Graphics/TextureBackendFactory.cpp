#include "Factories/Concretions/Context/Systems/Graphics/TextureBackendFactory.h"
#include "Factories/Products/Context/Systems/Graphics/SDLTextureBackend.h"


namespace DF2D::Factories
{
	std::unique_ptr<Core::ITextureBackend> TextureBackendFactory::CreateProduct()
	{
		return std::make_unique<Internal::SDLTextureBackend>();
	}
}