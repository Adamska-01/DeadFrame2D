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
		TEXT
	};
}