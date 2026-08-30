#pragma once


namespace DF2D::Data
{
	/** @brief Interaction states the backend tracks for an element and styling can select on. */
	enum class UIPseudoClass
	{
		HOVER,

		ACTIVE,

		FOCUS,

		DISABLED,

		CHECKED
	};
}