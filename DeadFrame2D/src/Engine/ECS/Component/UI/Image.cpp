#include "Engine/ECS/Component/UI/Image.h"
#include "Utilities/Helpers/UI/StyleValues.h"


namespace DF2D::Engine
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	Image::Image()
	{
	}


	void Image::OnElementCreated()
	{
		ApplySprite();
	}


	void Image::ApplySprite()
	{
		if (spriteSource.empty())
		{
			// No texture, so the element is a flat colour fill and needs no decorator at all.
			ClearStyle(UIStyleProperty::DECORATOR);

			// Only written when the game actually chose a colour. Inline properties outrank every
			// stylesheet rule, so stamping a default here would quietly make the element unthemeable
			// and kill any :hover or class-based background the game defined.
			if (colorOverridden)
			{
				SetStyle(UIStyleProperty::BACKGROUND_COLOR, color);
			}
			else
			{
				ClearStyle(UIStyleProperty::BACKGROUND_COLOR);
			}

			return;
		}

		// A decorator rather than an image element: it is the only form that supports slicing and
		// tiling, so all three image types go through one code path.
		// A decorator paints over the background, so any flat fill is cleared out of the way.
		ClearStyle(UIStyleProperty::BACKGROUND_COLOR);

		auto decorator = std::string();

		switch (imageType)
		{
		case UIImageType::SLICED:
			decorator = "ninepatch(\"" + spriteSource + "\", "
				+ std::to_string(sliceBorders.x) + "px "
				+ std::to_string(sliceBorders.y) + "px "
				+ std::to_string(sliceBorders.w) + "px "
				+ std::to_string(sliceBorders.h) + "px)";
			break;

		case UIImageType::TILED:
			decorator = "tiled-box(\"" + spriteSource + "\")";
			break;

		default:
			decorator = "image(\"" + spriteSource + "\")";
			break;
		}

		SetStyle(UIStyleProperty::DECORATOR, decorator);

		if (colorOverridden)
		{
			SetStyle(UIStyleProperty::IMAGE_COLOR, color);
		}
	}


	void Image::SetSprite(std::string_view texturePath)
	{
		auto newSource = std::string(texturePath);

		if (newSource == spriteSource)
			return;

		spriteSource = std::move(newSource);

		ApplySprite();
	}

	void Image::SetColor(const Color& newColor)
	{
		if (newColor == color && colorOverridden)
			return;

		color = newColor;
		colorOverridden = true;

		ApplySprite();
	}

	void Image::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		SetColor(Color{ r, g, b, a });
	}

	void Image::SetImageType(UIImageType type)
	{
		if (type == imageType)
			return;

		imageType = type;

		ApplySprite();
	}

	void Image::SetSliceBorders(const RectI& borders)
	{
		sliceBorders = borders;

		ApplySprite();
	}

	Color Image::GetColor() const
	{
		return color;
	}
}