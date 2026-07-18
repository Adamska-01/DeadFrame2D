#pragma once
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Frame lifecycle driven by the Input system. Implemented privately by
	 * the input sub-systems so only holders of this interface can advance frames.
	 */
	class DF2D_API IInputFrameLifecycle
	{
	public:
		virtual ~IInputFrameLifecycle() = default;


		virtual void BeginFrame() = 0;

		virtual void PreUpdate() = 0;
	};
}