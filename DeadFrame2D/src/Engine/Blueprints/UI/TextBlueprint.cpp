#include "Engine/Blueprints/UI/TextBlueprint.h"
#include "Constants/CommonColors.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	TextBlueprint::TextBlueprint(
		const std::string& value,
		float fontSize,
		const Color& color,
		UIAnchor anchor,
		const Vector2F& anchoredPosition,
		const Vector2F& size)
	{
		auto rectTransform = AddComponent<RectTransform>();
		rectTransform->SetAnchorPreset(anchor);
		rectTransform->SetAnchoredPosition(anchoredPosition);
		rectTransform->SetSizeDelta(size);

		text = AddComponent<Text>();
		text->SetText(value);
		text->SetFontSize(fontSize);
		text->SetColor(color);
		text->SetAlignment(TextAlignment::CENTER);
	}

	TextBlueprint::TextBlueprint(
		const std::string& value,
		float fontSize,
		const Vector2F& normalizedAnchor,
		float width)
	{
		// Point anchor (min == max) + centred pivot = exact placement, no offset math needed.
		auto rectTransform = AddComponent<RectTransform>();
		rectTransform->SetAnchorMin(normalizedAnchor);
		rectTransform->SetAnchorMax(normalizedAnchor);
		rectTransform->SetPivot({ 0.5f, 0.5f });
		rectTransform->SetSizeDelta({ width, fontSize * 1.6f });

		text = AddComponent<Text>();
		text->SetText(value);
		text->SetFontSize(fontSize);
		text->SetColor(Constants::CommonColors::WHITE);
		text->SetAlignment(TextAlignment::CENTER);
	}

	ComponentHandle<Text> TextBlueprint::GetText() const
	{
		return text;
	}
}