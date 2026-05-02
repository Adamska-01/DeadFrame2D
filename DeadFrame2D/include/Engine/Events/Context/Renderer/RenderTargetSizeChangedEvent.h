#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DF2D::Engine
{
	class DF2D_API RenderTargetSizeChangedEvent : public DispatchableEvent
	{
	public:
		Core::Vector2I renderTargetSize;


		RenderTargetSizeChangedEvent(Core::Vector2I renderTargetSize);
	};
}