#include "Engine/ECS/Component/GameComponent.h"
#include "Engine/Events/GameComponent/GameComponentAddedEvent.h"


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