#include "Constants/CommonColors.h"
#include "Core/SubSystems/Systems/Rendering/RenderSystem.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/UI/Canvas.h"
#include "Engine/Components/UI/Image.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Engine;


	Image::Image()
		: color(CommonColors::WHITE)
	{
	}

	void Image::Draw()
	{
		UIComponent::Draw();

		auto currentPosition = transform->GetWorldPosition();
		auto worldRotation = transform->GetWorldRotation();
		auto scaledSize = GetWidgetSize();
		auto anchorVector = GetAnchorFromPreset(anchor);

		auto destRect = SDL_FRect
		{
			currentPosition.x - (scaledSize.x * anchorVector.x),
			currentPosition.y - (scaledSize.y * anchorVector.y),
			scaledSize.x,
			scaledSize.y
		};

		renderTask.renderPhase = parentCanvas->GetRenderMode() == CanvasRenderMode::SCREEN_SPACE_CAMERA ? RenderPhase::SCREEN_SPACE_CAMERA_UI : RenderPhase::SCREEN_SPACE_OVERLAY_UI;
		renderTask.sortOrder = parentCanvas->GetSortOrder();
		renderTask.canvas = parentCanvas;

		if (sourceImage != nullptr)
		{
			renderTask.renderData = SpriteRenderData
			{
				.texture = sourceImage.get(),
				.destRect = destRect,
				.flip = SDL_RendererFlip::SDL_FLIP_NONE,
				.rotation = worldRotation,
				.colorMod = color
			};
		}
		else
		{
			renderTask.renderData = RectRenderData
			{
				.filled = true,
				.destRect = destRect,
				.rotation = worldRotation,
				.color = color
			};
		}

		RenderSystem::Submit(renderTask);
	}

	void Image::LoadSprite(std::string_view texturePath)
	{
		sourceImage = TextureManager::LoadTexture(texturePath);

		auto width = 0, height = 0;
		SDL_QueryTexture(sourceImage.get(), NULL, NULL, &width, &height);

		widgetSize.x = static_cast<float>(width);
		widgetSize.y = static_cast<float>(height);
	}

	void Image::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
}