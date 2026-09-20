#include "Engine/Blueprints/UI/ButtonBlueprint.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	ButtonBlueprint::ButtonBlueprint(
		const std::string& text,
		UIAnchor anchor,
		const Vector2F& anchoredPosition,
		const Vector2F& size)
	{
		auto rectTransform = AddComponent<RectTransform>();
		rectTransform->SetAnchorPreset(anchor);
		rectTransform->SetAnchoredPosition(anchoredPosition);
		rectTransform->SetSizeDelta(size);

		button = AddComponent<Button>();
		button->SetText(text);
	}

	ComponentHandle<Button> ButtonBlueprint::GetButton() const
	{
		return button;
	}
}