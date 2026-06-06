#pragma once
#include "Core/Context/Systems/Graphics/Abstractions/ITextureBackend.h"
#include <memory>


namespace DF2D::Factories
{
	class TextureBackendFactory
	{
	public:
		std::unique_ptr<Core::ITextureBackend> CreateProduct();
	};
}
