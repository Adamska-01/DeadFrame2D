#pragma once
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Consumer-facing half of the frame timer.
	 */
	class DF2D_API ITimeProvider
	{
	public:
		virtual ~ITimeProvider() = default;


		/**
		 * @brief Duration of the last frame in seconds, scaled by the current time scale.
		 */
		virtual float DeltaTime() const = 0;

		/**
		 * @brief Duration of the last frame in seconds, ignoring the current time scale.
		 */
		virtual float DeltaTimeUnscaled() const = 0;

		/**
		 * @brief Running frames-per-second estimate (frames / elapsed time since the last
		 * sample window reset), updated every frame and available from the first frame.
		 */
		virtual float Framerate() const = 0;


		/**
		 * @brief Caps the frame rate at the given value. Zero is ignored.
		 */
		virtual void SetTargetFramerate(unsigned int fps) = 0;

		/**
		 * @brief Removes the frame rate cap.
		 */
		virtual void UnlockFramerate() = 0;

		virtual bool IsFramerateLocked() const = 0;


		/**
		 * @brief Scales the frame delta seen by gameplay. Negative values clamp to zero.
		 */
		virtual void SetTimeScale(float scale) = 0;

		virtual float GetTimeScale() const = 0;
	};
}