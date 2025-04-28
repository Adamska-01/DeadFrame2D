#pragma once


class IInteractableUI;


/**
 * @brief Interface for UI elements that can be navigated using directional inputs.
 */
class INavigableUI
{
protected:
	/**
	 * @brief Pointer to the interactable UI element that is navigated to when moving up.
	 */
	IInteractableUI* up;

	/**
	 * @brief Pointer to the interactable UI element that is navigated to when moving down.
	 */
	IInteractableUI* down;

	/**
	 * @brief Pointer to the interactable UI element that is navigated to when moving right.
	 */
	IInteractableUI* right;

	/**
	 * @brief Pointer to the interactable UI element that is navigated to when moving left.
	 */
	IInteractableUI* left;


public:
	/**
	 * @brief Navigates the UI element upwards.
	 */
	virtual void NavigateUp() = 0;

	/**
	 * @brief Navigates the UI element downwards.
	 */
	virtual void NavigateDown() = 0;

	/**
	 * @brief Navigates the UI element rightwards.
	 */
	virtual void NavigateRight() = 0;

	/**
	 * @brief Navigates the UI element leftwards.
	 */
	virtual void NavigateLeft() = 0;


	/**
	* @brief Assigns the interactable UI element to be navigated to when moving up.
	* @param upElement Pointer to the interactable UI element to navigate to when moving up.
	*/
	void SetNavigableUpElement(IInteractableUI* upElement);

	/**
	 * @brief Assigns the interactable UI element to be navigated to when moving down.
	 * @param downElement Pointer to the interactable UI element to navigate to when moving down.
	 */
	void SetNavigableDownElement(IInteractableUI* downElement);

	/**
	 * @brief Assigns the interactable UI element to be navigated to when moving right.
	 * @param rightElement Pointer to the interactable UI element to navigate to when moving right.
	 */
	void SetNavigableRightElement(IInteractableUI* rightElement);

	/**
	 * @brief Assigns the interactable UI element to be navigated to when moving left.
	 * @param leftElement Pointer to the interactable UI element to navigate to when moving left.
	 */
	void SetNavigableLeftElement(IInteractableUI* leftElement);
};