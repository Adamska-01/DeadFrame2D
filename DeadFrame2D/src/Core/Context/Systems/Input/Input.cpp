#include "Constants/Paths/ResourcePaths.h"
#include "Core/Context/Systems/Input/Actions/InputActionResolver.h"
#include "Core/Context/Systems/Input/Devices/DeviceManager.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Input/User/InputUserManager.h"
#include "Models/Input/ActionMap/InputActionMapBucket.h"
#include "Utilities/IO/Serialization/JsonSerializer.h"


namespace DF2D::Core
{
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Models;
	using namespace DF2D::Utilities;


	Input::Input()
	{
		if (!JsonSerializer::IsSerializable<InputActionMapBucket>())
			throw std::runtime_error("[Input] InputActionMapBucket is not serializable. Cannot load input configuration...");

		auto actionMapBucket = JsonSerializer::DeserializeFromFile<InputActionMapBucket>(Paths::Files::INPUT_CONTROLS);

		userManager = std::make_unique<InputUserManager>();

		actionResolver = std::make_unique<InputActionResolver>(std::move(actionMapBucket), *userManager);

		deviceManager = std::make_unique<DeviceManager>(actionResolver.get());

		// Per-user action tables are managed through direct calls, not the event bus,
		// so their lifetime is ordered before any InputUserCreated/Destroyed broadcast.
		userManager->SetUserLifecycleHooks(
			[this](InputUserID userID)
			{
				actionResolver->AddUser(userID);
			},
			[this](InputUserID userID)
			{
				actionResolver->RemoveUser(userID);
			});

		// Same for device pairings: unpaired before the DeviceRemovedEvent broadcast.
		deviceManager->SetDeviceRemovedHook(
			[this](InputDeviceID deviceID)
			{
				userManager->UnpairDevice(userManager->GetUserFromPairedDevice(deviceID), deviceID);
			});
	}

	Input::~Input()
	{
	}

	void Input::OnSystemEvent(const SystemEvent& systemEvent)
	{
		deviceManager->HandleEvent(systemEvent);
	}

	void Input::BeginFrame()
	{
		// The call order here matters!
		actionResolver->BeginFrame();
		deviceManager->BeginFrame();
	}

	void Input::PreUpdate(float deltaTime)
	{
		actionResolver->PreUpdate();
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