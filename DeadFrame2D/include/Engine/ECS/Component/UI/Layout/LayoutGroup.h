#pragma once
#include "Data/Components/UI/Layout/LayoutPadding.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"


namespace DF2D::Engine
{
	/**
	 * @brief Base for containers that arrange their children instead of letting each place itself.
	 *
	 * A child of one of these no longer places itself: its RectTransform supplies the size and the
	 * group decides the position.
	 */
	class DF2D_API LayoutGroup : public UIComponent
	{
		TYPE_INFO(LayoutGroup, UIComponent);


	private:
		float spacing;

		Data::LayoutPadding padding;


		void ApplySpacing();

		void ApplyPadding();


	protected:
		void OnElementCreated() override;

		/** @brief Writes the properties that decide how children are arranged. */
		virtual void ApplyArrangement() = 0;


	public:
		LayoutGroup();

		virtual ~LayoutGroup() override = default;


		/** @brief Gap between children, in pixels. Safe to call before the component is initialised. */
		void SetSpacing(float value);

		/** @brief Inset between the container's edges and its children, in pixels. */
		void SetPadding(const Data::LayoutPadding& value);


		float GetSpacing() const;

		const Data::LayoutPadding& GetPadding() const;
	};
}