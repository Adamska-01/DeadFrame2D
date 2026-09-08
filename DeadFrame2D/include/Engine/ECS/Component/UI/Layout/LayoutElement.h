#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"


namespace DF2D::Engine
{
	/**
	 * @brief Overrides how a layout group sizes this element.
	 * 
	 * Only meaningful under a layout group. On an element that places itself there is no group to
	 * override, and nothing here applies.
	 */
	class DF2D_API LayoutElement : public UIComponent
	{
		TYPE_INFO(LayoutElement, UIComponent);


	private:
		float flexGrow = 0.0f;

		float flexShrink = 0.0f;

		Core::Vector2F minSize = Core::Vector2F::Zero;

		Core::Vector2F preferredSize = Core::Vector2F(-1.0f, -1.0f);

		bool ignoreLayout = false;


		void ApplyFlex();

		void ApplySizes();

		void RefreshPlacement();


	protected:
		void OnElementCreated() override;


	public:
		LayoutElement();

		virtual ~LayoutElement() override = default;


		/** @brief Share of any spare space along the group's axis this element takes. Zero takes none. */
		void SetFlexGrow(float value);

		/** @brief How readily this element gives up space when the group runs short. Zero gives none. */
		void SetFlexShrink(float value);

		/** @brief Smallest size the group may reduce this element to, in pixels. */
		void SetMinSize(const Core::Vector2F& value);

		/** @brief Size this element asks for, in pixels. A negative component leaves that axis to content. */
		void SetPreferredSize(const Core::Vector2F& value);

		/**
		 * @brief Takes this element out of the group's arrangement, so it places itself again.
		 *
		 * Its RectTransform positions it against the parent's box, on top of the arranged children.
		 */
		void SetIgnoreLayout(bool value);


		float GetFlexGrow() const;

		float GetFlexShrink() const;

		const Core::Vector2F& GetMinSize() const;

		const Core::Vector2F& GetPreferredSize() const;

		bool IsIgnoringLayout() const;
	};
}