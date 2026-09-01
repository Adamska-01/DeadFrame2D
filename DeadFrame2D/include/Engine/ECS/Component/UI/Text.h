#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Color.h"
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Text/FontStyle.h"
#include "Data/Components/UI/Text/TextAlignment.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include <string>
#include <string_view>


namespace DF2D::Engine
{
	/**
	 * @brief Draws a run of text in a UI element.
	 *
	 * Font size, weight and style are element properties rather than part of loading a font, so a
	 * single loaded face serves every size in the game and changing size costs nothing but a restyle.
	 */
	class DF2D_API Text : public UIComponent
	{
		TYPE_INFO(Text, UIComponent);


	private:
		std::string text;

		std::string fontFamily;

		float fontSize = 24.0f;

		Core::Color color = Constants::CommonColors::WHITE;

		Data::FontStyle fontStyle = Data::FontStyle::NORMAL;

		Data::TextAlignment alignment = Data::TextAlignment::LEFT;

		bool wordWrap = true;


		void ApplyTextStyle();


	protected:
		void OnElementCreated() override;


	public:
		Text();

		virtual ~Text() override = default;


		void SetText(std::string_view value);

		/** @brief Selects a loaded font by family name. Empty means inherit from the stylesheet. */
		void SetFontFamily(std::string_view family);

		void SetFontSize(float size);

		void SetColor(const Core::Color& color);

		void SetFontStyle(Data::FontStyle style);

		void SetAlignment(Data::TextAlignment value);

		void SetWordWrap(bool enabled);


		const std::string& GetText() const;

		float GetFontSize() const;

		Core::Color GetColor() const;

		/** @brief The size this text wants, for sizing an element to fit its content. */
		Core::Vector2F GetPreferredSize() const;
	};
}