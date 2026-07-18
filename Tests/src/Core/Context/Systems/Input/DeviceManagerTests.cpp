#include <doctest.h>
#include "Constants/Input/DefaultDeviceIDs.h"
#include "Core/Context/Systems/Input/Devices/DeviceManager.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/ControllerInputDevice.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/KeyboardInputDevice.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"
#include "Mocks/Context/Systems/Input/MockInputActionHandler.h"


using namespace DF2D::Constants;
using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("DeviceManager");


TEST_CASE("Keyboard and mouse always exist")
{
	MockInputActionHandler handler;
	DeviceManager manager(&handler);

	CHECK(manager.Keyboard() != nullptr);
	CHECK(manager.Mouse() != nullptr);
	CHECK(manager.GetDevice(DefaultDeviceIDs::KEYBOARD) == manager.Keyboard());
	CHECK(manager.GetDevice(DefaultDeviceIDs::MOUSE) == manager.Mouse());
	CHECK(manager.GetAllDevices().size() == 2);
}


TEST_CASE("Key events are routed to the keyboard device")
{
	MockInputActionHandler handler;
	DeviceManager manager(&handler);
	ISystemEventSink& sink = manager;

	sink.OnSystemEvent(KeyEvent{ KeyboardKeyCode::W, true });

	CHECK(manager.Keyboard()->GetButtonState(KeyboardKeyCode::W).pressed == true);
}


TEST_CASE("Mouse events are routed to the mouse device")
{
	MockInputActionHandler handler;
	DeviceManager manager(&handler);
	ISystemEventSink& sink = manager;

	sink.OnSystemEvent(MouseMoveEvent{ { 50.0f, 60.0f }, { 1.0f, 1.0f } });
	sink.OnSystemEvent(MouseButtonEvent{ MouseButtonCode::LEFT, true, { 50.0f, 60.0f } });

	auto position = manager.Mouse()->GetMousePosition();

	CHECK(position.x == doctest::Approx(50.0f));
	CHECK(position.y == doctest::Approx(60.0f));
	CHECK(manager.Mouse()->GetButtonState(MouseButtonCode::LEFT).pressed == true);
}


TEST_CASE("Controller connect creates a device and becomes current")
{
	MockInputActionHandler handler;
	DeviceManager manager(&handler);
	ISystemEventSink& sink = manager;

	sink.OnSystemEvent(ControllerConnectedEvent{ 5, "Pad" });

	auto* controller = manager.Controller(5);

	REQUIRE(controller != nullptr);
	CHECK(controller->Name() == "Pad");
	CHECK(manager.CurrentController() == controller);
	CHECK(manager.GetAllDevices().size() == 3);
}


TEST_CASE("Controller button events route by device id and update current")
{
	MockInputActionHandler handler;
	DeviceManager manager(&handler);
	ISystemEventSink& sink = manager;

	sink.OnSystemEvent(ControllerConnectedEvent{ 5, "PadOne" });
	sink.OnSystemEvent(ControllerConnectedEvent{ 9, "PadTwo" });

	sink.OnSystemEvent(ControllerButtonEvent{ 9, ControllerButtonCode::A, true });

	CHECK(manager.Controller(9)->GetButtonState(ControllerButtonCode::A).pressed == true);
	CHECK(manager.Controller(5)->GetButtonState(ControllerButtonCode::A).pressed == false);
	CHECK(manager.CurrentController() == manager.Controller(9));
}


TEST_CASE("Events for unknown controllers are ignored")
{
	MockInputActionHandler handler;
	DeviceManager manager(&handler);
	ISystemEventSink& sink = manager;

	sink.OnSystemEvent(ControllerButtonEvent{ 42, ControllerButtonCode::A, true });
	sink.OnSystemEvent(ControllerAxisEvent{ 42, ControllerAxisCode::LEFT_STICK_X, 0.5f });

	CHECK(manager.CurrentController() == nullptr);
	CHECK(handler.calls.empty());
}


TEST_CASE("Controller disconnect removes the device and clears current")
{
	MockInputActionHandler handler;
	DeviceManager manager(&handler);
	ISystemEventSink& sink = manager;

	sink.OnSystemEvent(ControllerConnectedEvent{ 5, "Pad" });
	sink.OnSystemEvent(ControllerDisconnectedEvent{ 5 });

	CHECK(manager.Controller(5) == nullptr);
	CHECK(manager.CurrentController() == nullptr);
	CHECK(manager.GetAllDevices().size() == 2);
}


TEST_CASE("Device removed hook fires with the removed id")
{
	MockInputActionHandler handler;

	std::vector<InputDeviceID> removed;

	DeviceManager manager(
		&handler,
		[&](InputDeviceID deviceID)
		{
			removed.push_back(deviceID);
		});

	ISystemEventSink& sink = manager;

	sink.OnSystemEvent(ControllerConnectedEvent{ 5, "Pad" });
	sink.OnSystemEvent(ControllerDisconnectedEvent{ 5 });

	REQUIRE(removed.size() == 1);
	CHECK(removed[0] == 5);
}


TEST_CASE("Controller lookups never alias keyboard or mouse ids")
{
	MockInputActionHandler handler;
	DeviceManager manager(&handler);

	CHECK(manager.Controller(DefaultDeviceIDs::KEYBOARD) == nullptr);
	CHECK(manager.Controller(DefaultDeviceIDs::MOUSE) == nullptr);
}


TEST_CASE("BeginFrame reaches every device")
{
	MockInputActionHandler handler;
	DeviceManager manager(&handler);
	ISystemEventSink& sink = manager;
	IInputFrameLifecycle& lifecycle = manager;

	sink.OnSystemEvent(KeyEvent{ KeyboardKeyCode::W, true });
	lifecycle.BeginFrame();

	CHECK(manager.Keyboard()->GetButtonState(KeyboardKeyCode::W).held == true);
}


TEST_SUITE_END();