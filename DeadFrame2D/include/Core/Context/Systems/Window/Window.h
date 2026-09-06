#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/UI/Abstractions/IUIPlatform.h"
#include "Core/Context/Systems/Window/Abstractions/IWindowBackend.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Window/WindowMode.h"
#include "DF2D_API.h"
#include <memory>


namespace DF2D::Core
{
	/**
	 * @brief The application window, and the platform services the UI needs from it.
	 */
	class DF2D_API Window : public ICoreSystem, public IUIPlatform
	{
	private:
		std::unique_ptr<IWindowBackend> backend;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		Window(std::unique_ptr<IWindowBackend> backend);

		~Window() override;

		Window(const Window&) = delete;

		Window(Window&&) = delete;

		Window& operator=(const Window&) = delete;

		Window& operator=(Window&&) = delete;


		void SetCursor(Data::CursorType cursor) override;

		void SetClipboardText(const std::string& text) override;

		std::string GetClipboardText() override;

		void StartTextInput(const RectI& caretRect) override;

		void StopTextInput() override;


		Vector2I GetResolution();

		void SetWindowTitle(const std::string& title);

		void SetWindowIcon(std::string_view iconSource);

		void SetWindowMode(Data::WindowMode mode);

		bool SetResolution(Vector2I resolution);
	};
}