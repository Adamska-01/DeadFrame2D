#pragma once
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Reports whether something is currently claiming an input for itself.
	 */
	class DF2D_API IInputCaptureState
	{
	public:
		virtual ~IInputCaptureState() = default;


		/** @brief Whether the pointer is claimed, so mouse-driven actions should not fire. */
		virtual bool CapturesPointer() const = 0;

		/** @brief Whether typed input is claimed, so key-driven actions should not fire. */
		virtual bool CapturesKeyboard() const = 0;
	};
}
