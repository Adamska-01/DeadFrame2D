#pragma once
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Core/Math/Vector2.h"


class RenderTargetSizeChangedEvent : public DispatchableEvent
{
public:
	Vector2I renderTargetSize;


	RenderTargetSizeChangedEvent(Vector2I renderTargetSize);
};