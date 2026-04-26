#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API RenderTargetSizeChangedEvent : public DispatchableEvent
	{
	public:
		DeadFrame2D::Core::Vector2I renderTargetSize;


		RenderTargetSizeChangedEvent(DeadFrame2D::Core::Vector2I renderTargetSize);
	};
}