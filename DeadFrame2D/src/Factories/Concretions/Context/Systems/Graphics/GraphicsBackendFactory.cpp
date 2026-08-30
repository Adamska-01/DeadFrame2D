#include "Factories/Concretions/Context/Systems/Graphics/GraphicsBackendFactory.h"
#include "Factories/Products/Context/Systems/Graphics/SDLTextureBackend.h"
#include "Factories/Products/Context/Systems/Rendering/SDLRenderBackend.h"
#include "Factories/Products/Context/Systems/Window/SDLWindowBackend.h"


namespace DF2D::Factories
{
	using namespace DF2D::Internal;
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Models;


	GraphicsBackendSet GraphicsBackendFactory::CreateProduct(
		const WindowConfig& windowConfig,
		const RendererConfig& rendererConfig,
		EventDispatcher& eventDispatcher)
	{
		auto windowBackend = std::make_unique<SDLWindowBackend>(windowConfig.title, windowConfig.width, windowConfig.height, windowConfig.fullscreen);

		auto renderBackend = std::make_unique<SDLRenderBackend>(windowBackend->GetSDLWindow(), rendererConfig, eventDispatcher);

		auto* sdlRenderer = renderBackend->GetSDLRenderer();
		auto* textureRegistry = renderBackend->GetTextureRegistry();

		auto textureBackend = std::make_unique<SDLTextureBackend>(sdlRenderer, textureRegistry);

		return GraphicsBackendSet
		{
			.windowBackend = std::move(windowBackend),
			.renderBackend = std::move(renderBackend),
			.textureBackend = std::move(textureBackend)
		};
	}
}