#pragma once
#include "Data/Input/PlayerInputSlot.h"
#include "DF2D_API.h"
#include <SDL_events.h>
#include <unordered_map>


namespace DeadFrame2D::Core
{
	class DF2D_API InputDevice
	{
	protected:
		DeadFrame2D::Data::PlayerInputSlot assignedPlayer;

		std::unordered_map<Uint8, bool> currentInputStates;

		std::unordered_map<Uint8, bool> lastInputStates;


		InputDevice();


	public:
		// TODO: Why not using IEventProcessor?
		virtual void ProcessEvent(const SDL_Event& controllerEvent) = 0;


		void BeginInputFrame();

		DeadFrame2D::Data::PlayerInputSlot GetAssignedPlayer() const;

		void AssignedPlayer(DeadFrame2D::Data::PlayerInputSlot assignedPlayer);

		bool IsKeyPressed(Uint8 inputKey) const;

		bool IsKeyHeld(Uint8 inputKey) const;
	};
}