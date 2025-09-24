#pragma once
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Core/Math/Vector2.h"


namespace DeadFrame2D::Engine
{
	class RenderTargetSizeChangedEvent : public DispatchableEvent
	{
	public:
		DeadFrame2D::Core::Vector2I renderTargetSize;


		RenderTargetSizeChangedEvent(DeadFrame2D::Core::Vector2I renderTargetSize);
	};
}