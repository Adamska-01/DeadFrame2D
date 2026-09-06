#pragma once
#include "Core/Context/Systems/Input/Abstractions/IInputCaptureSource.h"


struct MockInputCaptureSource : DF2D::Core::IInputCaptureSource
{
	bool capturesPointer{false};

	bool capturesKeyboard{false};


	bool CapturesPointer() const override
	{
		return capturesPointer;
	}

	bool CapturesKeyboard() const override
	{
		return capturesKeyboard;
	}
};