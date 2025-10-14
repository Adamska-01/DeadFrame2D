#pragma once
#include "DF2D_API.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Engine
{
	class IInteractableUI;


	/**
	 * @brief Interface for UI elements that can be navigated using directional inputs.
	 */
	class DF2D_API INavigableUI
	{
	protected:
		/**
		 * @brief Pointer to the interactable UI element that is navigated to when moving up.
		 */
		ComponentHandle<IInteractableUI> up;

		/**
		 * @brief Pointer to the interactable UI element that is navigated to when moving down.
		 */
		ComponentHandle<IInteractableUI> down;

		/**
		 * @brief Pointer to the interactable UI element that is navigated to when moving right.
		 */
		ComponentHandle<IInteractableUI> right;

		/**
		 * @brief Pointer to the interactable UI element that is navigated to when moving left.
		 */
		ComponentHandle<IInteractableUI> left;


	public:
		/**
		 * @brief Gets the UI element upwards.
		 */
		ComponentHandle<IInteractableUI> GetUpInteractable();

		/**
		 * @brief Gets the UI element downwards.
		 */
		ComponentHandle<IInteractableUI> GetDownInteractable();

		/**
		 * @brief Gets the UI element rightwards.
		 */
		ComponentHandle<IInteractableUI> GetRightInteractable();

		/**
		 * @brief Gets the UI element leftwards.
		 */
		ComponentHandle<IInteractableUI> GetLeftInteractable();


		/**
		* @brief Assigns the interactable UI element to be navigated to when moving up.
		* @param upElement Pointer to the interactable UI element to navigate to when moving up.
		*/
		void SetNavigableUpElement(ComponentHandle<IInteractableUI> upElement);

		/**
		 * @brief Assigns the interactable UI element to be navigated to when moving down.
		 * @param downElement Pointer to the interactable UI element to navigate to when moving down.
		 */
		void SetNavigableDownElement(ComponentHandle<IInteractableUI> downElement);

		/**
		 * @brief Assigns the interactable UI element to be navigated to when moving right.
		 * @param rightElement Pointer to the interactable UI element to navigate to when moving right.
		 */
		void SetNavigableRightElement(ComponentHandle<IInteractableUI> rightElement);

		/**
		 * @brief Assigns the interactable UI element to be navigated to when moving left.
		 * @param leftElement Pointer to the interactable UI element to navigate to when moving left.
		 */
		void SetNavigableLeftElement(ComponentHandle<IInteractableUI> leftElement);
	};
}