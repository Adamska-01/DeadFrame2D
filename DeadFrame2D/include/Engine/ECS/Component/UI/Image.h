#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Data/Components/UI/Image/UIImageType.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include <string>
#include <string_view>


namespace DF2D::Engine
{
	/**
	 * @brief Draws a texture, or a flat colour, in a UI element's box.
	 *
	 * With no sprite set the element is filled with the tint colour alone, which is the usual way to
	 * make a plain panel.
	 */
	class DF2D_API Image : public UIComponent
	{
		TYPE_INFO(Image, UIComponent);


	private:
		std::string spriteSource;

		Core::Color color = Constants::CommonColors::WHITE;

		/**
		 * @brief Whether the game set a colour, as opposed to this being the untouched default.
		 *
		 * An unset colour is left out of the element entirely so stylesheets keep control of it.
		 */
		bool colorOverridden = false;

		Data::UIImageType imageType = Data::UIImageType::SIMPLE;

		Core::RectI sliceBorders{};


		void ApplySprite();


	protected:
		void OnElementCreated() override;


	public:
		Image();

		virtual ~Image() override = default;


		/** @brief Sets the texture to draw. An empty path clears it back to a flat colour fill. */
		void SetSprite(std::string_view texturePath);

		/** @brief Tints the sprite, or fills the element when no sprite is set. */
		void SetColor(const Core::Color& color);

		void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		void SetImageType(Data::UIImageType type);

		/** @brief Corner sizes kept unstretched when the image type is sliced. */
		void SetSliceBorders(const Core::RectI& borders);


		Core::Color GetColor() const;
	};
}