#include "Core/Context/Systems/UI/Abstractions/IUIBackend.h"
#include "Engine/ECS/Component/UI/Text.h"
#include "Utilities/Helpers/UI/StyleValues.h"


namespace DF2D::Engine
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	namespace
	{
		const char* ToAlignmentValue(TextAlignment alignment)
		{
			switch (alignment)
			{
			case TextAlignment::CENTER:		return "center";
			case TextAlignment::RIGHT:		return "right";
			case TextAlignment::JUSTIFY:	return "justify";
			default:						return "left";
			}
		}
	}


	Text::Text()
	{
	}


	void Text::OnElementCreated()
	{
		ApplyTextStyle();

		if (auto* backend = Backend())
		{
			backend->SetElementText(element, text);
		}
	}

	void Text::ApplyTextStyle()
	{
		SetStyle(UIStyleProperty::FONT_SIZE, fontSize);
		SetStyle(UIStyleProperty::COLOR, color);
		SetStyle(UIStyleProperty::TEXT_ALIGN, ToAlignmentValue(alignment));
		SetStyle(UIStyleProperty::WHITE_SPACE, wordWrap ? "normal" : "nowrap");

		if (!fontFamily.empty())
		{
			SetStyle(UIStyleProperty::FONT_FAMILY, fontFamily);
		}

		SetStyle(UIStyleProperty::FONT_WEIGHT, HasFontStyle(fontStyle, FontStyle::BOLD) ? "bold" : "normal");
		SetStyle(UIStyleProperty::FONT_STYLE, HasFontStyle(fontStyle, FontStyle::ITALIC) ? "italic" : "normal");

		auto decoration = HasFontStyle(fontStyle, FontStyle::UNDERLINE)
			? "underline"
			: HasFontStyle(fontStyle, FontStyle::STRIKETHROUGH) ? "line-through" : "none";

		SetStyle(UIStyleProperty::TEXT_DECORATION, decoration);
	}


	void Text::SetText(std::string_view value)
	{
		auto newText = std::string(value);

		if (newText == text)
			return;

		text = std::move(newText);

		if (auto* backend = Backend())
		{
			backend->SetElementText(element, text);
		}
	}

	void Text::SetFontFamily(std::string_view family)
	{
		fontFamily = std::string(family);

		ApplyTextStyle();
	}

	void Text::SetFontSize(float size)
	{
		if (size == fontSize)
			return;

		fontSize = size;

		ApplyTextStyle();
	}

	void Text::SetColor(const Color& newColor)
	{
		if (newColor == color)
			return;

		color = newColor;

		ApplyTextStyle();
	}

	void Text::SetFontStyle(FontStyle style)
	{
		if (style == fontStyle)
			return;

		fontStyle = style;

		ApplyTextStyle();
	}

	void Text::SetAlignment(TextAlignment value)
	{
		if (value == alignment)
			return;

		alignment = value;

		ApplyTextStyle();
	}

	void Text::SetWordWrap(bool enabled)
	{
		if (enabled == wordWrap)
			return;

		wordWrap = enabled;

		ApplyTextStyle();
	}


	const std::string& Text::GetText() const
	{
		return text;
	}

	float Text::GetFontSize() const
	{
		return fontSize;
	}

	Color Text::GetColor() const
	{
		return color;
	}

	Vector2F Text::GetPreferredSize() const
	{
		auto* backend = Backend();

		return backend != nullptr ? backend->GetElementContentSize(element) : Vector2F::Zero;
	}
}