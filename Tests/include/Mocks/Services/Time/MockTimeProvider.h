#pragma once
#include "Core/Services/Time/Abstractions/ITimeProvider.h"


struct MockTimeProvider : DF2D::Core::ITimeProvider
{
	// Controllable state
	float deltaTime{0.0f};

	float deltaTimeUnscaled{0.0f};

	float timeScale{1.0f};

	float framerate{0.0f};

	bool isFramerateLocked{false};


	// Call tracking
	unsigned int lastTargetFramerate{0};

	int unlockFramerateCount{0};


	float DeltaTime() const override
	{
		return deltaTime;
	}

	float DeltaTimeUnscaled() const override
	{
		return deltaTimeUnscaled;
	}

	float Framerate() const override
	{
		return framerate;
	}


	void SetTargetFramerate(unsigned int fps) override
	{
		lastTargetFramerate = fps;
		isFramerateLocked = true;
	}

	void UnlockFramerate() override
	{
		unlockFramerateCount++;
		isFramerateLocked = false;
	}

	bool IsFramerateLocked() const override
	{
		return isFramerateLocked;
	}


	void SetTimeScale(float scale) override
	{
		timeScale = scale;
	}

	float GetTimeScale() const override
	{
		return timeScale;
	}
};