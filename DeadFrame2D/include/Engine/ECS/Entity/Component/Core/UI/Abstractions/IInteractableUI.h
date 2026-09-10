#pragma once
#include "Data/Components/UI/Navigation/UINavigationMode.h"
#include "Data/Systems/UI/UINavigationDirection.h"
#include "Data/Systems/UI/UIPseudoClass.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <array>
#include <string>


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

		bool focusRequested = false;

		Data::UINavigationMode navigationMode = Data::UINavigationMode::AUTOMATIC;

		/** @brief Named neighbour per direction, indexed by UINavigationDirection. Empty means unset. */
		std::array<std::string, 4> navigationTargets;

		/** @brief This widget's element id, assigned only once something navigates to it. */
		std::string navigationId;


		/** @brief Writes whether focus, and so navigation, may land on this widget. */
		void ApplyNavigability();


	protected:
		/** @brief Sealed: every interactable widget must end up focusable, whatever else it sets up. */
		void OnElementCreated() final;

		/** @brief Called once the element exists and is navigable, for a widget's own setup. */
		virtual void OnInteractableCreated();

		/** @brief Called after UI events have been dispatched to add custom behavior. */
		virtual void OnInteraction(Data::UIEventType eventType, const Data::UIEventPayload& payload);


		void HandleUIEvent(Data::UIEventType eventType, const Data::UIEventPayload& payload) override;


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

		/** @brief Gives this widget keyboard focus, taking it from whatever held it before. */
		void Focus();

		/** @brief Sets how focus moves away from this widget. */
		void SetNavigationMode(Data::UINavigationMode mode);

		/**
		 * @brief Sets the widget to focus in the given direction.
		 *
		 * Setting a target switches the navigation mode to EXPLICIT. Passing a null handle clears the target.
		 */
		void SetNavigationTarget(Data::UINavigationDirection direction, const ComponentHandle<IInteractableUI>& target);

		Data::UINavigationMode GetNavigationMode() const;

		/** @brief The element id other widgets navigate to this one by, assigned on first use. */
		const std::string& NavigationId();

		/** @brief Whether the pointer is currently over this element. */
		bool IsHovered() const;

		/** @brief Whether this element is currently being pressed. */
		bool IsPressed() const;

		/** @brief Whether this element holds keyboard focus. */
		bool IsFocused() const;
	};
}