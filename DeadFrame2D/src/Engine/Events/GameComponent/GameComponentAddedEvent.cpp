#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/Events/GameComponent/GameComponentAddedEvent.h"


namespace DF2D::Engine
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