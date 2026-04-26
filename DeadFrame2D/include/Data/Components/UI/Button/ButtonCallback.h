#pragma once
#include "Engine/ECS/Entity/Component/Handle/ComponentHandleBase.h"
#include <functional>


namespace DeadFrame2D::Data
{
	struct ButtonCallback
	{
		DeadFrame2D::Engine::ComponentHandleBase handle;

		std::function<void()> callback = []() {};
	};
}