#pragma once
#include "Engine/Entity/ComponentHandleBase.h"
#include <functional>


namespace DeadFrame2D::Data
{
	struct ButtonCallback
	{
		DeadFrame2D::Engine::ComponentHandleBase handle;

		std::function<void()> callback = []() {};
	};
}