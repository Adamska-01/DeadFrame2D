#pragma once
#include "Data/Systems/Graphics/GraphicsBackendSet.h"
#include "DF2D_API.h"
#include "Models/Rendering/RendererConfig.h"
#include "Models/Window/WindowConfig.h"


namespace DF2D::Engine
{
	class EventDispatcher;
}


namespace DF2D::Factories
{
	class DF2D_API GraphicsBackendFactory
	{
	public:
		Data::GraphicsBackendSet CreateProduct(
			const Models::WindowConfig& windowConfig,
			const Models::RendererConfig& rendererConfig,
			Engine::EventDispatcher& eventDispatcher);
	};
}