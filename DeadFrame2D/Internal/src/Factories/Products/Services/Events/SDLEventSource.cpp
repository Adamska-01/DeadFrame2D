#include "Constants/Input/DefaultDeviceNames.h"
#include "Constants/Input/InputConstants.h"
#include "Converters/Input/ControllerButtonConversions.h"
#include "Converters/Input/KeyboardKeyCodeConversions.h"
#include "Converters/Input/MouseButtonConversions.h"
#include "Factories/Products/Services/Events/SDLEventSource.h"
#include <algorithm>
#include <iostream>
#include <SDL.h>


namespace DF2D::Internal
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Models;


	SDLEventSource::SDLEventSource()
	{
		// IMPORTANT: If you want to disable RawInput/XInput correlation, set the hint
		// BEFORE initializing the controller subsystem.
		SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT, "0");

		if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0)
		{
			if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
			{
				std::cerr << "[Input] Failed to init SDL_GAMECONTROLLER: " << SDL_GetError() << std::endl;
			}
			else
			{
				std::cout << "[Info] Input subsystem successfully initialized." << std::endl;
			}
		}
	}

	SDLEventSource::~SDLEventSource()
	{
		for (auto& [instanceID, controller] : openControllers)
		{
			SDL_GameControllerClose(controller);
		}

		openControllers.clear();

		if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 0)
		{
			SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);

			std::cout << "[Info] Input subsystem successfully shut down." << std::endl;
		}
	}

	std::optional<SystemEvent> SDLEventSource::PollEvent()
	{
		SDL_Event sdlEvent;

		while (SDL_PollEvent(&sdlEvent))
		{
			if (auto translated = Translate(sdlEvent))
				return translated;
		}

		return std::nullopt;
	}

	void SDLEventSource::PushQuitEvent()
	{
		auto toSend = SDL_Event
		{
			.type = SDL_QUIT
		};

		SDL_PushEvent(&toSend);
	}

	std::optional<SystemEvent> SDLEventSource::Translate(const SDL_Event& sdlEvent)
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			return QuitEvent{};

		case SDL_WINDOWEVENT:
			if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
				return QuitEvent{};

			return std::nullopt;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			return KeyEvent
			{
				.key = KeyboardKeyCodeConversions::ToKeyboardKeyCode(sdlEvent.key.keysym.scancode),
				.pressed = sdlEvent.type == SDL_KEYDOWN
			};

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			return MouseButtonEvent
			{
				.button = MouseButtonConversions::ToMouseButtonCode(sdlEvent.button.button),
				.pressed = sdlEvent.type == SDL_MOUSEBUTTONDOWN,
				.position = Vector2F(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y))
			};

		case SDL_MOUSEMOTION:
			return MouseMoveEvent
			{
				.position = Vector2F(static_cast<float>(sdlEvent.motion.x), static_cast<float>(sdlEvent.motion.y)),
				.delta = Vector2F(static_cast<float>(sdlEvent.motion.xrel), static_cast<float>(sdlEvent.motion.yrel))
			};

		case SDL_MOUSEWHEEL:
			return MouseWheelEvent
			{
				.delta = Vector2F(static_cast<float>(sdlEvent.wheel.x), static_cast<float>(sdlEvent.wheel.y))
			};

		case SDL_CONTROLLERDEVICEADDED:
			return OpenController(sdlEvent.cdevice.which);

		case SDL_CONTROLLERDEVICEREMOVED:
			return CloseController(sdlEvent.cdevice.which);

		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			return ControllerButtonEvent
			{
				.deviceID = sdlEvent.cbutton.which,
				.button = ControllerButtonConversions::ToControllerButtonCode(static_cast<SDL_GameControllerButton>(sdlEvent.cbutton.button)),
				.pressed = sdlEvent.type == SDL_CONTROLLERBUTTONDOWN
			};

		case SDL_CONTROLLERAXISMOTION:
			return ControllerAxisEvent
			{
				.deviceID = sdlEvent.caxis.which,
				.axis = ControllerButtonConversions::ToControllerAxisCode(static_cast<SDL_GameControllerAxis>(sdlEvent.caxis.axis)),
				.value = std::clamp(static_cast<float>(sdlEvent.caxis.value) / InputConstants::AXIS_MAX_VALUE, -1.0f, 1.0f)
			};

		default:
			return std::nullopt;
		}
	}

	std::optional<SystemEvent> SDLEventSource::OpenController(int deviceIndex)
	{
		if (!SDL_IsGameController(deviceIndex))
			return std::nullopt;

		auto controller = SDL_GameControllerOpen(deviceIndex);

		if (controller == nullptr)
		{
			std::cerr << "[Input] Failed to open controller index " << deviceIndex << ": " << SDL_GetError() << std::endl;

			return std::nullopt;
		}

		auto joystick = SDL_GameControllerGetJoystick(controller);
		auto instanceID = SDL_JoystickInstanceID(joystick);

		openControllers[instanceID] = controller;

		auto name = SDL_GameControllerName(controller);

		return ControllerConnectedEvent
		{
			.deviceID = instanceID,
			.name = name != nullptr ? name : DefaultDeviceNames::CONTROLLER
		};
	}

	std::optional<SystemEvent> SDLEventSource::CloseController(InputDeviceID instanceID)
	{
		auto it = openControllers.find(instanceID);

		if (it == openControllers.end())
			return std::nullopt;

		SDL_GameControllerClose(it->second);

		openControllers.erase(it);

		return ControllerDisconnectedEvent
		{
			.deviceID = instanceID
		};
	}
}
