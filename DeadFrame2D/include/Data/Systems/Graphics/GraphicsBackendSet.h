#pragma once
#include "Core/Context/Systems/Graphics/Abstractions/ITextureBackend.h"
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Window/Abstractions/IWindowBackend.h"
#include <memory>


namespace DF2D::Data
{
	struct GraphicsBackendSet
	{
		std::unique_ptr<Core::IWindowBackend> windowBackend;

		std::unique_ptr<Core::IRenderBackend> renderBackend;

		std::unique_ptr<Core::ITextureBackend> textureBackend;
	};
}