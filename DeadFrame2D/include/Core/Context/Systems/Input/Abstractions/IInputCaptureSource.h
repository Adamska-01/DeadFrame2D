#pragma once
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Reports which input the UI is currently claiming for itself.
	 */
	class DF2D_API IInputCaptureSource
	{
	public:
		virtual ~IInputCaptureSource() = default;


		/** @brief Whether the pointer is over UI, so mouse-driven actions should not fire. */
		virtual bool CapturesPointer() const = 0;

		/** @brief Whether a UI element is taking typed input, so key-driven actions should not fire. */
		virtual bool CapturesKeyboard() const = 0;
	};
}