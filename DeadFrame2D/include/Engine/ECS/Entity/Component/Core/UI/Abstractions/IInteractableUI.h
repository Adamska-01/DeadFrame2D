#pragma once
#include "Data/Systems/UI/UIPseudoClass.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Utilities/Delegates/MulticastDelegate.h"


namespace DF2D::Engine
{
	/**
	 * @brief Base for UI components a player can interact with.
	 *
	 * Interaction state is not tracked here. The UI backend already owns hit testing, press tracking
	 * and focus, so this reads that state back rather than watching the pointer itself.
	 */
	class DF2D_API IInteractableUI : public UIComponent
	{
		TYPE_INFO(IInteractableUI, UIComponent);


	private:
		bool interactable = true;


	protected:
		void HandleUIEvent(Data::UIEventType eventType, const Data::UIEventPayload& payload) override;

		/**
		 * @brief Called after UI events have been dispatched to add custom behavior.
		 */
		virtual void OnInteraction(Data::UIEventType eventType, const Data::UIEventPayload& payload);


	public:
		virtual ~IInteractableUI() override = default;


		/** @brief Fired when this element is clicked. */
		Utilities::MultiCastVoid OnClick;

		/** @brief Fired when the pointer moves onto this element. */
		Utilities::MultiCastVoid OnPointerEnter;

		/** @brief Fired when the pointer leaves this element. */
		Utilities::MultiCastVoid OnPointerExit;

		/** @brief Fired when a pointer button goes down on this element. */
		Utilities::MultiCastVoid OnPointerDown;

		/** @brief Fired when a pointer button is released over this element. */
		Utilities::MultiCastVoid OnPointerUp;


		/**
		 * @brief Enables or disables interaction.
		 *
		 * A disabled element still lays out and draws; it stops receiving pointer events and gains a
		 * disabled state that stylesheets can select on.
		 */
		void SetInteractable(bool value);

		bool IsInteractable() const;

		/** @brief Whether the pointer is currently over this element. */
		bool IsHovered() const;

		/** @brief Whether this element is currently being pressed. */
		bool IsPressed() const;

		/** @brief Whether this element holds keyboard focus. */
		bool IsFocused() const;
	};
}