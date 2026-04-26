#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Engine
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