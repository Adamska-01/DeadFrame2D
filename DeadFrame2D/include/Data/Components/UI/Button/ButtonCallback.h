#pragma once
#include "Engine/ECS/Entity/Component/Handle/ComponentHandleBase.h"
#include <functional>


namespace DF2D::Data
{
	struct ButtonCallback
	{
		Engine::ComponentHandleBase handle;

		std::function<void()> callback = []() {};
	};
}