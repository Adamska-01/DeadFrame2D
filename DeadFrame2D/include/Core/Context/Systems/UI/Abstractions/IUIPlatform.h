#pragma once
#include "Core/Math/Rect.h"
#include "Data/Systems/UI/CursorType.h"
#include "DF2D_API.h"
#include <string>


namespace DF2D::Core
{
	/**
	 * @brief The window-level services the UI backend needs from the platform.
	 */
	class DF2D_API IUIPlatform
	{
	public:
		virtual ~IUIPlatform() = default;


		/**
		 * @brief Sets the mouse cursor shape.
		 *
		 * Driven by whatever the UI is hovering, so it changes many times per second and implementations
		 * are expected to cache rather than rebuild a cursor per call.
		 */
		virtual void SetCursor(Data::CursorType cursor) = 0;

		/** @brief Replaces the system clipboard contents. */
		virtual void SetClipboardText(const std::string& text) = 0;

		/**
		 * @brief Reads the system clipboard.
		 *
		 * @return The clipboard text, or an empty string when it is empty or holds something else.
		 */
		virtual std::string GetClipboardText() = 0;

		/**
		 * @brief Begins text entry, so the platform can place an IME or on-screen keyboard.
		 *
		 * Called when a text field takes focus. Until this runs, the platform has no reason to be
		 * producing text input events at all, which is why it is a distinct step from focusing.
		 *
		 * @param caretRect: Caret rectangle in window space, for IME candidate placement.
		 */
		virtual void StartTextInput(const RectI& caretRect) = 0;

		/** @brief Ends text entry, dismissing any IME or on-screen keyboard. */
		virtual void StopTextInput() = 0;
	};
}
