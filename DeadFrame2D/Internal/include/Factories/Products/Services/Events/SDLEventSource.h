#pragma once
#include "Core/Services/Events/Abstractions/IEventSource.h"
#include "Data/Systems/Input/InputDeviceID.h"
#include "DF2D_API.h"
#include <unordered_map>


struct _SDL_GameController;

typedef struct _SDL_GameController SDL_GameController;

union SDL_Event;


namespace DF2D::Internal
{
	class DF2D_API SDLEventSource : public Core::IEventSource
	{
	private:
		std::unordered_map<Data::InputDeviceID, SDL_GameController*> openControllers;


		std::optional<Data::SystemEvent> Translate(const SDL_Event& sdlEvent);

		std::optional<Data::SystemEvent> OpenController(int deviceIndex);

		std::optional<Data::SystemEvent> CloseController(Data::InputDeviceID instanceID);


	public:
		SDLEventSource();

		~SDLEventSource() override;

		SDLEventSource(const SDLEventSource&) = delete;

		SDLEventSource(SDLEventSource&&) = delete;

		SDLEventSource& operator=(const SDLEventSource&) = delete;

		SDLEventSource& operator=(SDLEventSource&&) = delete;


		std::optional<Data::SystemEvent> PollEvent() override;

		void PushQuitEvent() override;
	};
}