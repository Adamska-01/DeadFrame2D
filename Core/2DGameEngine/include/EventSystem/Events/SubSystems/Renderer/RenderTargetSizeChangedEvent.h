#pragma once
#include "EventSystem/DispatchableEvent.h"
#include "Math/Vector2.h"


class RenderTargetSizeChangedEvent : public DispatchableEvent
{
public:
	Vector2I renderTargetSize;


	RenderTargetSizeChangedEvent(Vector2I renderTargetSize);
};