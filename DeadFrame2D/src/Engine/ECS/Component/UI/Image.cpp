#include "Constants/CommonColors.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Component/UI/Image.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"


namespace DF2D::Engine
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Engine;


	Image::Image()
		: color(CommonColors::WHITE)
	{
	}

	void Image::Init()
	{
		UIComponent::Init();

		textureManager = GetGameObject()->CoreContext().textureManager;
	}

	void Image::Draw()
	{
		UIComponent::Draw();

		auto currentPosition = transform->GetWorldPosition();
		auto worldRotation = transform->GetWorldRotation();
		auto scaledSize = GetWidgetSize();
		auto anchorVector = GetAnchorFromPreset(anchor);

		auto destRect = Core::RectF
		{
			.x = currentPosition.x - (scaledSize.x * anchorVector.x),
			.y = currentPosition.y - (scaledSize.y * anchorVector.y),
			.w = scaledSize.x,
			.h = scaledSize.y
		};

		renderTask.renderPhase = parentCanvas->GetRenderMode() == CanvasRenderMode::SCREEN_SPACE_CAMERA ? RenderPhase::SCREEN_SPACE_CAMERA_UI : RenderPhase::SCREEN_SPACE_OVERLAY_UI;
		renderTask.sortOrder = parentCanvas->GetSortOrder();
		renderTask.canvas = parentCanvas;

		if (sourceImage != 0)
		{
			renderTask.renderData = SpriteRenderData
			{
				.texture = sourceImage,
				.destRect = destRect,
				.flip = RenderFlip::NONE,
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

	void Image::SetColor(Color color)
	{
		this->color = color;
	}

	void Image::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
}