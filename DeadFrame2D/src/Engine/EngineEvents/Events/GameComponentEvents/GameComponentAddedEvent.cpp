#include "Engine/Components/GameComponent.h"
#include "Engine/EngineEvents/Events/GameComponentEvents/GameComponentAddedEvent.h"


namespace DeadFrame2D::Engine
{
	GameComponentAddedEvent::GameComponentAddedEvent(const ComponentHandle<GameComponent>& componentHandle)
		: componentHandle(componentHandle)
	{
	}


	const ComponentHandle<GameComponent>& GameComponentAddedEvent::GetGameComponentHandle() const
	{
		return componentHandle;
	}
}