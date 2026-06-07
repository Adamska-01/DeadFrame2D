#include "Constants/CommonColors.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Component/UI/Image.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Utilities;


	Image::Image()
		: color(CommonColors::WHITE)
	{
	}

	void Image::Init()
	{
		UIComponent::Init();

		textureManager = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().textureManager, NAME_OF(textureManager));
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

		if (sourceImage != 0)
		{
			renderTask.renderData = SpriteRenderData
			{
				.texture = textureManager->GetRawTexture(sourceImage),
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
		if (!textureManager)
			return;

		sourceImage = textureManager->LoadTexture(texturePath);

		auto size = textureManager->GetTextureSize(sourceImage);

		widgetSize.x = static_cast<float>(size.x);
		widgetSize.y = static_cast<float>(size.y);
	}

	void Image::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
}