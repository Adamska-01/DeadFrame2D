#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/INavigableUI.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"


namespace DF2D::Engine
{
	/**
	 * @brief Represents the interactable status of a UI element.
	 */
	class DF2D_API IInteractableUI : public UIComponent, public INavigableUI
	{
		TYPE_INFO(IInteractableUI, UIComponent);


	protected:
		bool isPressed = false;

		bool isHovered = false;


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
}