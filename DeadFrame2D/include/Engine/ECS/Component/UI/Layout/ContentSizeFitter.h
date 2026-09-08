#pragma once
#include "Data/Components/UI/Layout/SizeFitMode.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"


namespace DF2D::Engine
{
	/**
	 * @brief Sizes an element to its content instead of to the size it was given.
	 *
	 * Set per axis, so a fixed width with a height that follows the text works. An axis left
	 * unconstrained keeps the size sizeDelta gave it.
	 *
	 * The RectTransform still writes the size; this only changes how it is worked out. A LayoutElement
	 * on the same object writes the size itself and overrides this.
	 */
	class DF2D_API ContentSizeFitter : public UIComponent
	{
		TYPE_INFO(ContentSizeFitter, UIComponent);


	private:
		Data::SizeFitMode horizontalFit = Data::SizeFitMode::UNCONSTRAINED;

		Data::SizeFitMode verticalFit = Data::SizeFitMode::UNCONSTRAINED;


		void RefreshPlacement();


	protected:
		void OnElementCreated() override;


	public:
		ContentSizeFitter();

		virtual ~ContentSizeFitter() override = default;


		void SetHorizontalFit(Data::SizeFitMode value);

		void SetVerticalFit(Data::SizeFitMode value);


		Data::SizeFitMode GetHorizontalFit() const;

		Data::SizeFitMode GetVerticalFit() const;
	};
}