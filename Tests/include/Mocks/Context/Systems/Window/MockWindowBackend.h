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
};
