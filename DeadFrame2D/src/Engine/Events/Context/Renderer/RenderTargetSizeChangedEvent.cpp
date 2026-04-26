#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;


	RenderTargetSizeChangedEvent::RenderTargetSizeChangedEvent(Vector2I renderTargetSize)
		: renderTargetSize(renderTargetSize)
	{
	}
}