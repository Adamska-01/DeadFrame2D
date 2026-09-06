#pragma once
#include "Core/Context/Systems/Input/Abstractions/IInputCaptureState.h"


struct MockInputCaptureState : DF2D::Core::IInputCaptureState
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