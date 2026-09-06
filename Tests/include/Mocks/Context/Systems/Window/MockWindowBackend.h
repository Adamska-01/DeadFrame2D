#pragma once
#include "Core/Context/Systems/Window/Abstractions/IWindowBackend.h"


struct MockWindowBackend : DF2D::Core::IWindowBackend
{
	// Configurable results
	DF2D::Core::Vector2I resolution{1920, 1080};

	bool setResolutionResult{true};


	// Call tracking
	int setResolutionCount{0};

	std::string lastTitle;

	std::string lastIconSource;

	DF2D::Data::WindowMode lastMode{DF2D::Data::WindowMode::WINDOWED};

	// UI platform services
	DF2D::Data::CursorType lastCursor{DF2D::Data::CursorType::ARROW};

	int setCursorCount{0};

	std::string clipboard;

	bool textInputActive{false};

	DF2D::Core::RectI lastCaretRect{};

	DF2D::Core::Vector2I lastRequestedResolution{};


	DF2D::Core::Vector2I GetResolution() override
	{
		return resolution;
	}

	void SetWindowTitle(const std::string& title) override
	{
		lastTitle = title;
	}

	void SetWindowIcon(std::string_view iconSource) override
	{
		lastIconSource = std::string(iconSource);
	}

	void SetWindowMode(DF2D::Data::WindowMode mode) override
	{
		lastMode = mode;
	}

	bool SetResolution(DF2D::Core::Vector2I newResolution) override
	{
		setResolutionCount++;
		lastRequestedResolution = newResolution;

		if (setResolutionResult)
			resolution = newResolution;

		return setResolutionResult;
	}

	void SetCursor(DF2D::Data::CursorType cursor) override
	{
		lastCursor = cursor;
		setCursorCount++;
	}

	void SetClipboardText(const std::string& text) override
	{
		clipboard = text;
	}

	std::string GetClipboardText() override
	{
		return clipboard;
	}

	void StartTextInput(const DF2D::Core::RectI& caretRect) override
	{
		textInputActive = true;
		lastCaretRect = caretRect;
	}

	void StopTextInput() override
	{
		textInputActive = false;
	}
};