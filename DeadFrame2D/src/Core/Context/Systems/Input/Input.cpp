#include "Core/Context/Systems/Input/Abstractions/IInputCaptureState.h"
#include "Core/Context/Systems/Input/Actions/InputActionResolver.h"
#include "Core/Context/Systems/Input/Devices/DeviceManager.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Input/User/InputUser.h"
#include "Core/Context/Systems/Input/User/InputUserManager.h"
#include "Models/Input/ActionMap/InputActionMapBucket.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Models;


	Input::Input(
		InputActionMapBucket actionMapBucket,
		EventDispatcher& eventDispatcher,
		const IInputCaptureState* captureState)
	{
		// Hooks capture this and are only invoked after construction completes.
		// Per-user action tables are managed through direct calls, not the event bus,
		// so their lifetime is ordered before any InputUserCreated/Destroyed broadcast.
		userManager = std::make_unique<InputUserManager>(
			eventDispatcher,
			[this](InputUserID userID)
			{
				actionResolver->AddUser(userID);

				// A controller already plugged in when the user appears belongs to that user.
				PairUnpairedControllers(userID);
			},
			[this](InputUserID userID)
			{
				actionResolver->RemoveUser(userID);
			});

		actionResolver = std::make_unique<InputActionResolver>(std::move(actionMapBucket), *userManager, captureState);

		// Same for device pairings: unpaired before the DeviceRemovedEvent broadcast.
		deviceManager = std::make_unique<DeviceManager>(
			actionResolver.get(),
			eventDispatcher,
			[this](InputDeviceID deviceID)
			{
				PairControllerToFreeUser(deviceID);
			},
			[this](InputDeviceID deviceID)
			{
				userManager->UnpairDevice(userManager->GetUserFromPairedDevice(deviceID), deviceID);
			});
	}

	Input::~Input()
	{
	}


	void Input::PairUnpairedControllers(InputUserID userID)
	{
		auto* user = userManager->GetUser(userID);

		if (user == nullptr || deviceManager == nullptr)
			return;

		for (auto* device : deviceManager->GetAllDevices())
		{
			if (device == nullptr || device->Type() != Models::InputDeviceType::CONTROLLER)
				continue;

			if (userManager->GetUserFromPairedDevice(device->ID()) != nullptr)
				continue;

			userManager->PairDeviceToUser(user, device->ID());
		}
	}

	void Input::PairControllerToFreeUser(InputDeviceID deviceID)
	{
		auto* device = deviceManager != nullptr ? deviceManager->GetDevice(deviceID) : nullptr;

		if (device == nullptr || device->Type() != Models::InputDeviceType::CONTROLLER)
			return;

		if (userManager->GetUserFromPairedDevice(deviceID) != nullptr)
			return;

		// The first user without a controller takes it, so plugging one in mid-game reaches whoever is
		// playing rather than waiting for a user that will never be created.
		for (auto* user : userManager->GetAllUsers())
		{
			auto hasController = false;

			for (auto pairedID : userManager->GetDevicesPairedToUser(user->ID()))
			{
				auto* paired = deviceManager->GetDevice(pairedID);

				hasController = hasController || (paired != nullptr && paired->Type() == Models::InputDeviceType::CONTROLLER);
			}

			if (!hasController)
			{
				userManager->PairDeviceToUser(user, deviceID);

				return;
			}
		}
	}


	void Input::OnSystemEvent(const SystemEvent& systemEvent)
	{
		ISystemEventSink& deviceSink = *deviceManager;

		deviceSink.OnSystemEvent(systemEvent);
	}

	void Input::BeginFrame()
	{
		IInputFrameLifecycle& resolverLifecycle = *actionResolver;
		IInputFrameLifecycle& deviceLifecycle = *deviceManager;

		// The call order here matters!
		resolverLifecycle.BeginFrame();
		deviceLifecycle.BeginFrame();
	}

	void Input::PreUpdate(float deltaTime)
	{
		IInputFrameLifecycle& resolverLifecycle = *actionResolver;

		resolverLifecycle.PreUpdate();
	}

	void Input::EndUpdate(float deltaTime)
	{
	}

	void Input::EndDraw()
	{
	}

	IInputDeviceProvider* Input::Devices()
	{
		return deviceManager.get();
	}

	IInputUsers* Input::Users()
	{
		return userManager.get();
	}

	IInputActions* Input::Actions()
	{
		return actionResolver.get();
	}
}