#include <doctest.h>
#include "Constants/Input/DefaultDeviceIDs.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"
#include "Mocks/Context/Systems/Input/MockInputActionHandler.h"


using namespace DF2D::Constants;
using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("MouseInputDevice");


TEST_CASE("Identity is mouse with the default device id")
{
	MockInputActionHandler handler;
	MouseInputDevice mouse(&handler);

	CHECK(mouse.Type() == InputDeviceType::MOUSE);
	CHECK(mouse.ID() == DefaultDeviceIDs::MOUSE);
}


TEST_CASE("Button press and release follow the pressed/held/released cycle")
{
	MockInputActionHandler handler;
	MouseInputDevice mouse(&handler);
	InputDevice& device = mouse;

	CHECK(device.HandleEvent(MouseButtonEvent{ MouseButtonCode::LEFT, true, { 10.0f, 20.0f } }) == true);

	auto state = mouse.GetButtonState(MouseButtonCode::LEFT);
	CHECK(state.pressed == true);
	CHECK(state.value == doctest::Approx(1.0f));

	device.BeginFrame();

	state = mouse.GetButtonState(MouseButtonCode::LEFT);
	CHECK(state.pressed == false);
	CHECK(state.held == true);

	device.HandleEvent(MouseButtonEvent{ MouseButtonCode::LEFT, false, { 10.0f, 20.0f } });

	state = mouse.GetButtonState(MouseButtonCode::LEFT);
	CHECK(state.released == true);

	device.BeginFrame();

	state = mouse.GetButtonState(MouseButtonCode::LEFT);
	CHECK(state.released == false);
}


TEST_CASE("Move updates absolute position and accumulates motion delta")
{
	MockInputActionHandler handler;
	MouseInputDevice mouse(&handler);
	InputDevice& device = mouse;

	device.HandleEvent(MouseMoveEvent{ { 100.0f, 200.0f }, { 5.0f, -3.0f } });
	device.HandleEvent(MouseMoveEvent{ { 110.0f, 195.0f }, { 10.0f, -5.0f } });

	auto position = mouse.GetMousePosition();
	CHECK(position.x == doctest::Approx(110.0f));
	CHECK(position.y == doctest::Approx(195.0f));

	auto delta = mouse.GetMouseDelta();
	CHECK(delta.x == doctest::Approx(15.0f));
	CHECK(delta.y == doctest::Approx(-8.0f));
}


TEST_CASE("BeginFrame resets motion delta but keeps absolute position")
{
	MockInputActionHandler handler;
	MouseInputDevice mouse(&handler);
	InputDevice& device = mouse;

	device.HandleEvent(MouseMoveEvent{ { 100.0f, 200.0f }, { 5.0f, 5.0f } });
	device.BeginFrame();

	auto delta = mouse.GetMouseDelta();
	CHECK(delta.x == doctest::Approx(0.0f));
	CHECK(delta.y == doctest::Approx(0.0f));

	auto position = mouse.GetMousePosition();
	CHECK(position.x == doctest::Approx(100.0f));
	CHECK(position.y == doctest::Approx(200.0f));
}


TEST_CASE("Button events also refresh the cursor position")
{
	MockInputActionHandler handler;
	MouseInputDevice mouse(&handler);
	InputDevice& device = mouse;

	device.HandleEvent(MouseButtonEvent{ MouseButtonCode::LEFT, true, { 42.0f, 24.0f } });

	auto position = mouse.GetMousePosition();
	CHECK(position.x == doctest::Approx(42.0f));
	CHECK(position.y == doctest::Approx(24.0f));
}


TEST_CASE("Wheel delta accumulates and resets on BeginFrame")
{
	MockInputActionHandler handler;
	MouseInputDevice mouse(&handler);
	InputDevice& device = mouse;

	device.HandleEvent(MouseWheelEvent{ { 0.0f, 1.0f } });
	device.HandleEvent(MouseWheelEvent{ { 0.0f, 2.0f } });

	auto wheel = mouse.GetWheelDelta();
	CHECK(wheel.y == doctest::Approx(3.0f));

	device.BeginFrame();

	wheel = mouse.GetWheelDelta();
	CHECK(wheel.y == doctest::Approx(0.0f));
}


TEST_CASE("Motion notifies the action handler per axis")
{
	MockInputActionHandler handler;
	MouseInputDevice mouse(&handler);
	InputDevice& device = mouse;

	device.HandleEvent(MouseMoveEvent{ { 10.0f, 10.0f }, { 4.0f, 7.0f } });

	CHECK(handler.CountCallsFor(static_cast<int>(MouseAxisCode::MOTION_X)) == 1);
	CHECK(handler.CountCallsFor(static_cast<int>(MouseAxisCode::MOTION_Y)) == 1);
}


TEST_CASE("Non-mouse events are not consumed")
{
	MockInputActionHandler handler;
	MouseInputDevice mouse(&handler);
	InputDevice& device = mouse;

	CHECK(device.HandleEvent(KeyEvent{ KeyboardKeyCode::SPACE, true }) == false);
	CHECK(device.HandleEvent(ControllerAxisEvent{ 3, ControllerAxisCode::LEFT_STICK_X, 0.5f }) == false);

	CHECK(handler.calls.empty());
}


TEST_SUITE_END();