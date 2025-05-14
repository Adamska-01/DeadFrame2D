#include "Components/Rendering/ImageScroller.h"
#include "Components/Transform.h"
#include "Constants/ScreenConstants.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/SubSystems/Renderer/RenderTargetSizeChangedEvent.h"
#include "SubSystems/TextureManager.h"
#include "Tools/Helpers/EventHelpers.h"


ImageScroller::ImageScroller(std::string_view textureSource, ScrollDirection scrollDirection, float scrollSpeed)
	: Sprite(textureSource), scrollDirection(scrollDirection), scrollSpeed(scrollSpeed)
{
	renderTargetSize.x = ScreenConstants::DEFAULT_RENDERER_X_SIZE;
	renderTargetSize.y = ScreenConstants::DEFAULT_RENDERER_Y_SIZE;

	scrollOffset = 0;

	EventDispatcher::RegisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), EventHelpers::BindFunction(this, &ImageScroller::RenderTargetSizeChangedHandler), reinterpret_cast<std::uintptr_t>(this));
}

ImageScroller::~ImageScroller()
{
	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), reinterpret_cast<std::uintptr_t>(this));
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
			auto destRect = SDL_Rect
			{
				.x = x * scaledTileWidth - (isHorizontal ? static_cast<int>(scrollOffset) : 0) + static_cast<int>(position.x),
				.y = y * scaledTileHeight - (isHorizontal ? 0 : static_cast<int>(scrollOffset)) + static_cast<int>(position.y),
				.w = scaledTileWidth,
				.h = scaledTileHeight
			};

			TextureManager::DrawTexture(spriteTexture, NULL, &destRect, transform->GetWorldRotation());
		}
	}
}