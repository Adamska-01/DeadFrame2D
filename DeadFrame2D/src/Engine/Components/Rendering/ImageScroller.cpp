#include "Core/SubSystems/Systems/Rendering/Renderer.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Engine/Components/Rendering/ImageScroller.h"
#include "Engine/Components/Transform.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Renderer/RenderTargetSizeChangedEvent.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Helpers/Events/EventHelpers.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	ImageScroller::ImageScroller(std::string_view textureSource, ScrollDirection scrollDirection, float scrollSpeed)
		: SpriteRenderer(textureSource), scrollDirection(scrollDirection), scrollSpeed(scrollSpeed)
	{
		renderTargetSize = Renderer::GetResolutionTarget();

		scrollOffset = 0;

		EventDispatcher::RegisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), this, &ImageScroller::RenderTargetSizeChangedHandler);
	}

	ImageScroller::~ImageScroller()
	{
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), this);
	}

	void ImageScroller::RenderTargetSizeChangedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto renderTargetChangeEvent = DispatchableEvent::SafeCast<RenderTargetSizeChangedEvent>(dispatchableEvent);

		if (renderTargetChangeEvent == nullptr)
			return;

		renderTargetSize.x = renderTargetChangeEvent->renderTargetSize.x;
		renderTargetSize.y = renderTargetChangeEvent->renderTargetSize.y;
	}

	void ImageScroller::Update(float deltaTime)
	{
		scrollOffset += scrollSpeed * deltaTime;

		auto scale = transform->GetWorldScale();
		auto scaledTileWidth = static_cast<int>(spriteSize.x * scale.x);
		auto scaledTileHeight = static_cast<int>(spriteSize.y * scale.y);

		scrollOffset += scrollSpeed * deltaTime;

		if (scrollDirection == ScrollDirection::HORIZONTAL)
		{
			if (scrollOffset >= scaledTileWidth)
			{
				scrollOffset -= scaledTileWidth;
			}
			else if (scrollOffset < 0)
			{
				scrollOffset += scaledTileWidth;
			}
		}
		else
		{
			if (scrollOffset >= scaledTileHeight)
			{
				scrollOffset -= scaledTileHeight;
			}
			else if (scrollOffset < 0)
			{
				scrollOffset += scaledTileHeight;
			}
		}
	}

	void ImageScroller::Draw()
	{
		if (spriteTexture == nullptr)
			return;

		auto position = transform->GetWorldPosition();
		auto scale = transform->GetWorldScale();

		// Calculate scaled size
		auto scaledTileWidth = static_cast<int>(spriteSize.x * scale.x);
		auto scaledTileHeight = static_cast<int>(spriteSize.y * scale.y);

		// +2 to handle partial tiles at edges
		auto tilesX = (renderTargetSize.x / scaledTileWidth) + 2;
		auto tilesY = (renderTargetSize.y / scaledTileHeight) + 2;

		auto isHorizontal = scrollDirection == ScrollDirection::HORIZONTAL;

		for (auto y = 0; y < (isHorizontal ? 1 : tilesY); ++y)
		{
			for (auto x = 0; x < (isHorizontal ? tilesX : 1); ++x)
			{
				auto destRect = SDL_FRect
				{
					.x = x * scaledTileWidth - (isHorizontal ? scrollOffset : 0) + position.x,
					.y = y * scaledTileHeight - (isHorizontal ? 0 : scrollOffset) + position.y,
					.w = static_cast<float>(scaledTileWidth),
					.h = static_cast<float>(scaledTileHeight)
				};

				renderTask.renderData = SpriteRenderData
				{
					.texture = spriteTexture.get(),
					.destRect = destRect,
					.rotation = transform->GetWorldRotation(),
				};

				RenderSystem::Submit(renderTask);
			}
		}
	}
}