#pragma once
#include "Components/UI/Abstractions/INavigableUI.h"
#include "Components/UI/Abstractions/UIComponent.h"


/**
 * @brief Represents the interactable status of a UI element.
 */
class IInteractableUI : public UIComponent, public INavigableUI
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
	virtual bool IsPressed() const;

	/**
	 * @brief Checks if the UI element is currently hovered.
	 * @return True if hovered, false otherwise.
	 */
	virtual bool IsHovered() const;
};