#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/UI/UIElementID.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	class UIManager;


	/**
	 * @brief The scrolling view of an element, reached through UIElement::AsScrollable.
	 *
	 * Kept apart from UIElement for the same reason as UIDropdown: scrolling only means anything on a
	 * container that clips its content, so asking for it is a deliberate step rather than something
	 * every element offers.
	 */
	class DF2D_API UIScrollable
	{
		friend class UIElement;


	private:
		UIManager* manager = nullptr;

		Data::UIElementID id = 0;


		UIScrollable(UIManager* manager, Data::UIElementID id);


	public:
		UIScrollable() = default;


		bool IsValid() const;


		/** @brief How far the content is currently scrolled. */
		Vector2F GetScrollOffset() const;

		/** @brief Scrolls the content, clamped by the backend to what there is to scroll. */
		void SetScrollOffset(Vector2F offset);

		/** @brief The full size of the content, which is what it scrolls within. */
		Vector2F GetScrollSize() const;
	};
}
