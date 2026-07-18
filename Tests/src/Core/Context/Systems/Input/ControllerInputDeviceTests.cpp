#include <doctest.h>
#include "Core/Context/Systems/Input/Devices/DeviceTypes/ControllerInputDevice.h"
#include "Mocks/Context/Systems/Input/MockInputActionHandler.h"


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("ControllerInputDevice");


TEST_CASE("Identity reflects constructor arguments")
{
	MockInputActionHandler handler;
	ControllerInputDevice controller(3, "Test Pad", &handler);

	CHECK(controller.Type() == InputDeviceType::CONTROLLER);
	CHECK(controller.ID() == 3);
	CHECK(controller.Name() == "Test Pad");
}


TEST_CASE("Button press and release follow the pressed/held/released cycle")
{
	MockInputActionHandler handler;
	ControllerInputDevice controller(3, "Test Pad", &handler);
	InputDevice& device = controller;

	CHECK(device.HandleEvent(ControllerButtonEvent{ 3, ControllerButtonCode::A, true }) == true);

	auto state = controller.GetButtonState(ControllerButtonCode::A);
	CHECK(state.pressed == true);
	CHECK(state.value == doctest::Approx(1.0f));

	device.BeginFrame();

	state = controller.GetButtonState(ControllerButtonCode::A);
	CHECK(state.held == true);
	CHECK(state.pressed == false);

	device.HandleEvent(ControllerButtonEvent{ 3, ControllerButtonCode::A, false });

	state = controller.GetButtonState(ControllerButtonCode::A);
	CHECK(state.released == true);

	device.BeginFrame();

	state = controller.GetButtonState(ControllerButtonCode::A);
	CHECK(state.released == false);
	CHECK(state.value == doctest::Approx(0.0f));
}


TEST_CASE("Axis motion above the threshold counts as pressed then held")
{
	MockInputActionHandler handler;
	ControllerInputDevice controller(3, "Test Pad", &handler);
	InputDevice& device = controller;

	device.HandleEvent(ControllerAxisEvent{ 3, ControllerAxisCode::LEFT_STICK_X, 0.75f });

	auto state = controller.GetAxisState(ControllerAxisCode::LEFT_STICK_X);
	CHECK(state.pressed == true);
	CHECK(state.held == true);
	CHECK(state.value == doctest::Approx(0.75f));

	device.HandleEvent(ControllerAxisEvent{ 3, ControllerAxisCode::LEFT_STICK_X, 0.8f });

	state = controller.GetAxisState(ControllerAxisCode::LEFT_STICK_X);
	CHECK(state.pressed == false);
	CHECK(state.held == true);
}


TEST_CASE("Axis returning to rest is released")
{
	MockInputActionHandler handler;
	ControllerInputDevice controller(3, "Test Pad", &handler);
	InputDevice& device = controller;

	device.HandleEvent(ControllerAxisEvent{ 3, ControllerAxisCode::LEFT_STICK_X, 0.75f });
	device.HandleEvent(ControllerAxisEvent{ 3, ControllerAxisCode::LEFT_STICK_X, 0.0f });

	auto state = controller.GetAxisState(ControllerAxisCode::LEFT_STICK_X);
	CHECK(state.released == true);
	CHECK(state.held == false);
}


TEST_CASE("Events for other device ids are not consumed")
{
	MockInputActionHandler handler;
	ControllerInputDevice controller(3, "Test Pad", &handler);
	InputDevice& device = controller;

	CHECK(device.HandleEvent(ControllerButtonEvent{ 9, ControllerButtonCode::A, true }) == false);
	CHECK(device.HandleEvent(ControllerAxisEvent{ 9, ControllerAxisCode::LEFT_STICK_X, 0.5f }) == false);

	CHECK(controller.GetButtonState(ControllerButtonCode::A).pressed == false);
	CHECK(handler.calls.empty());
}


TEST_CASE("Non-controller events are not consumed")
{
	MockInputActionHandler handler;
	ControllerInputDevice controller(3, "Test Pad", &handler);
	InputDevice& device = controller;

	CHECK(device.HandleEvent(KeyEvent{ KeyboardKeyCode::SPACE, true }) == false);

	CHECK(handler.calls.empty());
}


TEST_CASE("Notifies the action handler for buttons and axes")
{
	MockInputActionHandler handler;
	ControllerInputDevice controller(3, "Test Pad", &handler);
	InputDevice& device = controller;

	device.HandleEvent(ControllerButtonEvent{ 3, ControllerButtonCode::A, true });
	device.HandleEvent(ControllerAxisEvent{ 3, ControllerAxisCode::LEFT_STICK_X, 0.5f });

	REQUIRE(handler.calls.size() == 2);
	CHECK(handler.calls[0].controlType == InputControlType::DIGITAL);
	CHECK(handler.calls[1].controlType == InputControlType::ANALOG);
	CHECK(handler.calls[1].deviceID == 3);
}


TEST_SUITE_END();