#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;


	RenderTargetSizeChangedEvent::RenderTargetSizeChangedEvent(Vector2I renderTargetSize)
		: renderTargetSize(renderTargetSize)
	{
	}
}