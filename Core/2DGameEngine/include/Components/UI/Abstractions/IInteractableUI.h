#pragma once
#include "Components/GameComponent.h"


/**
 * @brief Represents the interactable status of a UI element.
 */
class IInteractableUI : public GameComponent
{
protected:
	bool isPressed;

	bool isHovered;


public:
	virtual ~IInteractableUI() = default;


	/**
	 * @brief Called when the pointer enters the UI element.
	 */
	virtual void OnPointerEnter() = 0;

	/**
	 * @brief Called when the pointer exits the UI element.
	 */
	virtual void OnPointerExit() = 0;

	/**
	 * @brief Called when the pointer is pressed down on the UI element.
	 */
	virtual void OnPointerDown() = 0;

	/**
	 * @brief Called when the pointer is released on the UI element.
	 */
	virtual void OnPointerUp() = 0;


	/**
	 * @brief Checks if the UI element is currently pressed.
	 * @return True if pressed, false otherwise.
	 */
	virtual bool IsPressed() const = 0;

	/**
	 * @brief Checks if the UI element is currently hovered.
	 * @return True if hovered, false otherwise.
	 */
	virtual bool IsHovered() const = 0;
};