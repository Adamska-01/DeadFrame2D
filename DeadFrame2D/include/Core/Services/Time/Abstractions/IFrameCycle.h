#pragma once
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Frame-loop driving half of the frame timer. Implemented privately and
	 * used only by the runtime; consumers see ITimeProvider instead.
	 */
	class DF2D_API IFrameCycle
	{
	public:
		virtual ~IFrameCycle() = default;


		/**
		 * @brief Marks the start of a frame.
		 */
		virtual void BeginFrame() = 0;

		/**
		 * @brief Measures the frame, updates the framerate sample, and waits out any unused frame budget.
		 */
		virtual void EndFrame() = 0;
	};
}