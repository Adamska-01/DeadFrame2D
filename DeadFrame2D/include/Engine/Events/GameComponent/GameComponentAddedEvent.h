#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DF2D::Engine
{
	class GameComponent;


	class DF2D_API GameComponentAddedEvent : public DispatchableEvent
	{
	private:
		ComponentHandle<GameComponent> componentHandle;


	public:
		GameComponentAddedEvent(const ComponentHandle<GameComponent>& componentHandle);


		const ComponentHandle<GameComponent>& GetGameComponentHandle() const;
	};
}