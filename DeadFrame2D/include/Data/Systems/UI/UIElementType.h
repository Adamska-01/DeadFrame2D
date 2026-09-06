#pragma once


namespace DF2D::Data
{
	/**
	 * @brief The kind of element a UI component asks the backend to create.
	 */
	enum class UIElementType
	{
		/** @brief Generic container with no behaviour of its own. */
		PANEL,

		/** @brief Displays a texture. */
		IMAGE,

		/** @brief Displays a run of text. */
		TEXT,

		/** @brief Clickable button carrying its own pressed and hovered states. */
		BUTTON,

		/** @brief Two-state control that reports when it is switched on or off. */
		CHECKBOX,

		/** @brief Draggable control producing a number between a minimum and a maximum. */
		RANGE,

		/** @brief Single-line editable text. */
		TEXT_INPUT,

		/** @brief Multi-line editable text. */
		TEXT_AREA,

		/** @brief Closed list of options, one of which is selected. */
		DROPDOWN,

		/** @brief One option inside a dropdown. */
		DROPDOWN_OPTION,

		/** @brief Container that clips its content and scrolls it. */
		SCROLL_PANEL
	};
}