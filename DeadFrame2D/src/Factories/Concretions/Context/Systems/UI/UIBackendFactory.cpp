#include "Factories/Concretions/Context/Systems/UI/UIBackendFactory.h"
#include "Factories/Products/Context/Systems/UI/RmlUIBackend.h"


namespace DF2D::Factories
{
	using namespace DF2D::Core;
	using namespace DF2D::Internal;


	std::unique_ptr<IUIBackend> UIBackendFactory::CreateProduct(TextureManager* textureManager, IUIPlatform* platform)
	{
		return std::make_unique<RmlUIBackend>(textureManager, platform);
	}
}