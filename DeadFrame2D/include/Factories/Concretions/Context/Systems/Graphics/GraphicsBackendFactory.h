#pragma once
#include "Models/Rendering/RendererConfig.h"
#include "Models/Window/WindowConfig.h"
#include "Data/Systems/Graphics/GraphicsBackendSet.h"


namespace DF2D::Engine
{
	class EventDispatcher;
}


namespace DF2D::Factories
{
	class GraphicsBackendFactory
	{
	public:
		Data::GraphicsBackendSet CreateProduct(
			const Models::WindowConfig& windowConfig,
			const Models::RendererConfig& rendererConfig,
			Engine::EventDispatcher& eventDispatcher);
	};
}