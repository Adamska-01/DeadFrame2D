#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Utilities/Delegates/MulticastDelegate.h"


namespace DF2D::Engine
{
	/**
	 * @brief A container that clips its content and lets the player scroll through it.
	 *
	 * The content is ordinary child GameObjects. Nothing is nested inside this component: what scrolls
	 * is whatever the hierarchy puts under it, so a scroll view is a plain container that happens to
	 * clip.
	 */
	class DF2D_API ScrollView : public UIComponent
	{
		TYPE_INFO(ScrollView, UIComponent);


	private:
		bool horizontal = false;

		bool vertical = true;


		void ApplyOverflow();


	protected:
		Data::UIElementType GetElementType() const override;

		void OnElementCreated() override;

		void HandleUIEvent(Data::UIEventType eventType, const Data::UIEventPayload& payload) override;


	public:
		ScrollView();

		virtual ~ScrollView() override = default;


		/** @brief Fired when the content moves, carrying the new offset. */
		Utilities::MulticastDelegate<Core::Vector2F> OnScroll;


		/** @brief Allows scrolling across. Safe to call before the component is initialised. */
		void SetHorizontal(bool enabled);

		/** @brief Allows scrolling down. Safe to call before the component is initialised. */
		void SetVertical(bool enabled);

		/** @brief Moves the content. The backend clamps the offset to what there is to scroll. */
		void SetScrollPosition(Core::Vector2F position);


		bool IsHorizontal() const;

		bool IsVertical() const;

		Core::Vector2F GetScrollPosition() const;

		/** @brief The full size of the content, which is what the view scrolls within. */
		Core::Vector2F GetContentSize() const;
	};
}