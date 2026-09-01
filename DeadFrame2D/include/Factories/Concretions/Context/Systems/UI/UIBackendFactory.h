#pragma once
#include "Core/Context/Systems/UI/Abstractions/IUIBackend.h"
#include "DF2D_API.h"
#include <memory>


namespace DF2D::Core
{
	class IUIPlatform;
	class TextureManager;
}


namespace DF2D::Factories
{
	/**
	 * @brief Builds the UI backend.
	 *
	 * Separate from GraphicsBackendFactory because the UI backend depends on an assembled core system
	 * (the texture manager, for its cache and path handling) rather than on sibling backends, so it
	 * cannot be created alongside them.
	 */
	class DF2D_API UIBackendFactory
	{
	public:
		std::unique_ptr<Core::IUIBackend> CreateProduct(Core::TextureManager* textureManager, Core::IUIPlatform* platform);
	};
}