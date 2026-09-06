#pragma once
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/UI/CursorType.h"
#include "Data/Systems/Window/WindowMode.h"
#include "DF2D_API.h"
#include <string>


namespace DF2D::Core
{
	/**
	 * @brief Everything the engine needs from the application window.
	 *
	 * The first group is the window itself; the second is the things that belong to the window but are
	 * really asked for by the UI (cursor, clipboard, typing).
	 */
	class DF2D_API IWindowBackend
	{
	public:
		virtual ~IWindowBackend() = default;


		/**
		* @brief Current size of the window's drawable area, in pixels.
		*/
		virtual Core::Vector2I GetResolution() = 0;

		/**
		* @brief Sets the text shown in the title bar.
		*/
		virtual void SetWindowTitle(const std::string& title) = 0;

		/**
		* @brief Sets the window and taskbar icon from an image file.
		*/
		virtual void SetWindowIcon(std::string_view iconSource) = 0;

		/**
		* @brief Switches between windowed, borderless and fullscreen.
		*/
		virtual void SetWindowMode(Data::WindowMode mode) = 0;

		/**
		 * @brief Resizes the window.
		 *
		 * @return Whether the size was accepted; a display may refuse an unsupported one.
		 */
		virtual bool SetResolution(Core::Vector2I resolution) = 0;

		/**
		 * @brief Sets the mouse cursor shape.
		 */
		virtual void SetCursor(Data::CursorType cursor) = 0;

		/**
		* @brief Puts text on the system clipboard, for copy.
		*/
		virtual void SetClipboardText(const std::string& text) = 0;

		/**
		* @brief Reads the system clipboard, for paste. Empty if it holds no text.
		*/
		virtual std::string GetClipboardText() = 0;

		/**
		 * @brief Starts typing: the window begins sending text input events.
		 *
		 * Should be called when a text field gains focus. Until it runs, keys arrive but no typed text does.
		 *
		 * @param caretRect: Where the caret is on screen, so an on-screen keyboard or an IME
		 *                   candidate list can be placed next to it instead of over the text.
		 */
		virtual void StartTextInput(const Core::RectI& caretRect) = 0;

		/**
		* @brief Stops typing, dismissing any on-screen keyboard.
		*/
		virtual void StopTextInput() = 0;
	};
}