#include <doctest.h>
#include "Constants/Input/DefaultDeviceIDs.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/KeyboardInputDevice.h"
#include "Mocks/Context/Systems/Input/MockInputActionHandler.h"


using namespace DF2D::Constants;
using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("KeyboardInputDevice");


TEST_CASE("Identity is keyboard with the default device id")
{
	MockInputActionHandler handler;
	KeyboardInputDevice keyboard(&handler);

	CHECK(keyboard.Type() == InputDeviceType::KEYBOARD);
	CHECK(keyboard.ID() == DefaultDeviceIDs::KEYBOARD);
}


TEST_CASE("Key press sets pressed state and notifies the action handler")
{
	MockInputActionHandler handler;
	KeyboardInputDevice keyboard(&handler);
	InputDevice& device = keyboard;

	CHECK(device.HandleEvent(KeyEvent{ KeyboardKeyCode::SPACE, true }) == true);

	auto state = keyboard.GetButtonState(KeyboardKeyCode::SPACE);

	CHECK(state.pressed == true);
	CHECK(state.held == false);
	CHECK(state.value == doctest::Approx(1.0f));

	REQUIRE(handler.calls.size() == 1);
	CHECK(handler.calls[0].controlType == InputControlType::DIGITAL);
	CHECK(handler.calls[0].controlID == static_cast<int>(KeyboardKeyCode::SPACE));
}


TEST_CASE("BeginFrame promotes pressed to held")
{
	MockInputActionHandler handler;
	KeyboardInputDevice keyboard(&handler);
	InputDevice& device = keyboard;

	device.HandleEvent(KeyEvent{ KeyboardKeyCode::SPACE, true });
	device.BeginFrame();

	auto state = keyboard.GetButtonState(KeyboardKeyCode::SPACE);

	CHECK(state.pressed == false);
	CHECK(state.held == true);
}


TEST_CASE("Repeated press while held does not re-trigger")
{
	MockInputActionHandler handler;
	KeyboardInputDevice keyboard(&handler);
	InputDevice& device = keyboard;

	device.HandleEvent(KeyEvent{ KeyboardKeyCode::SPACE, true });
	device.BeginFrame();

	handler.calls.clear();

	device.HandleEvent(KeyEvent{ KeyboardKeyCode::SPACE, true }); // key repeat

	CHECK(handler.calls.empty());
	CHECK(keyboard.GetButtonState(KeyboardKeyCode::SPACE).held == true);
}


TEST_CASE("Release sets released for one frame, then clears")
{
	MockInputActionHandler handler;
	KeyboardInputDevice keyboard(&handler);
	InputDevice& device = keyboard;

	device.HandleEvent(KeyEvent{ KeyboardKeyCode::SPACE, true });
	device.BeginFrame();

	device.HandleEvent(KeyEvent{ KeyboardKeyCode::SPACE, false });

	auto state = keyboard.GetButtonState(KeyboardKeyCode::SPACE);

	CHECK(state.released == true);
	CHECK(state.held == false);
	CHECK(state.value == doctest::Approx(0.0f));

	device.BeginFrame();

	state = keyboard.GetButtonState(KeyboardKeyCode::SPACE);

	CHECK(state.released == false);
	CHECK(state.pressed == false);
	CHECK(state.held == false);
}


TEST_CASE("Out-of-range codes are safe")
{
	MockInputActionHandler handler;
	KeyboardInputDevice keyboard(&handler);
	InputDevice& device = keyboard;

	device.HandleEvent(KeyEvent{ KeyboardKeyCode::COUNT_MAX, true });

	CHECK(handler.calls.empty());

	auto state = keyboard.GetButtonState(-1);

	CHECK(state.value == doctest::Approx(0.0f));
	CHECK(state.pressed == false);
}


TEST_CASE("Non-keyboard events are not consumed")
{
	MockInputActionHandler handler;
	KeyboardInputDevice keyboard(&handler);
	InputDevice& device = keyboard;

	CHECK(device.HandleEvent(MouseWheelEvent{ { 0.0f, 1.0f } }) == false);
	CHECK(device.HandleEvent(ControllerButtonEvent{ 3, ControllerButtonCode::A, true }) == false);

	CHECK(handler.calls.empty());
}


TEST_CASE("Axis queries return empty state")
{
	MockInputActionHandler handler;
	KeyboardInputDevice keyboard(&handler);

	auto state = keyboard.GetAxisState(0);

	CHECK(state.value == doctest::Approx(0.0f));
}


TEST_SUITE_END();