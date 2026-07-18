#include "Core/Context/Systems/Input/Actions/InputActionResolver.h"
#include "Mocks/Context/Systems/Input/MockInputActionHandler.h"
#include "Mocks/Context/Systems/Input/MockInputDevice.h"
#include "Mocks/Context/Systems/Input/MockUserDevicePairings.h"
#include "Models/Input/ActionMap/InputActionMapBucket.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Models;


namespace
{
	constexpr InputUserID USER = 7;

	constexpr InputDeviceID DEVICE = -1;

	constexpr int JUMP_KEY = static_cast<int>(KeyboardKeyCode::SPACE);

	constexpr int LEFT_KEY = static_cast<int>(KeyboardKeyCode::A);

	constexpr int RIGHT_KEY = static_cast<int>(KeyboardKeyCode::D);

	constexpr int UP_KEY = static_cast<int>(KeyboardKeyCode::W);

	constexpr int DOWN_KEY = static_cast<int>(KeyboardKeyCode::S);


	InputActionMapBucket MakeBucket()
	{
		auto map = InputActionMap
		{
			.name = "Default",
			.actions =
			{
				InputAction{ .name = "Jump", .valueType = ValueType::BOOL },
				InputAction{ .name = "Move", .valueType = ValueType::FLOAT },
				InputAction{ .name = "Look", .valueType = ValueType::VECTOR2 }
			},
			.bindings =
			{
				Binding
				{
					.name = "JumpKey",
					.action = "Jump",
					.input = InputBinding
					{
						.inputDeviceType = InputDeviceType::KEYBOARD,
						.bindingType = BindingType::SIMPLE,
						.controlType = InputControlType::DIGITAL,
						.value = JUMP_KEY
					}
				},
				Binding
				{
					.name = "MoveKeys",
					.action = "Move",
					.input = InputBinding
					{
						.inputDeviceType = InputDeviceType::KEYBOARD,
						.bindingType = BindingType::COMPOSITE_1D,
						.controlType = InputControlType::DIGITAL,
						.value = Composite1D{ .negative = LEFT_KEY, .positive = RIGHT_KEY }
					}
				},
				Binding
				{
					.name = "LookKeys",
					.action = "Look",
					.input = InputBinding
					{
						.inputDeviceType = InputDeviceType::KEYBOARD,
						.bindingType = BindingType::COMPOSITE_2D,
						.controlType = InputControlType::DIGITAL,
						.value = Composite2D{ .up = UP_KEY, .down = DOWN_KEY, .left = LEFT_KEY, .right = RIGHT_KEY }
					}
				}
			}
		};

		return InputActionMapBucket{ .actionMaps = { map } };
	}


	struct ResolverFixture
	{
		MockUserDevicePairings pairings;

		MockInputActionHandler deviceHandler;

		MockInputDevice device{ &deviceHandler };

		InputActionResolver resolver{ MakeBucket(), pairings };


		ResolverFixture()
		{
			pairings.pairings[DEVICE] = USER;

			resolver.AddUser(USER);
			resolver.EnableActionMap(USER, "Default");
		}


		// Simulates one engine frame: begin, feed active controls, resolve
		void Frame(const std::vector<int>& activeControls)
		{
			IInputFrameLifecycle& lifecycle = resolver;
			IInputActionHandler& handler = resolver;

			lifecycle.BeginFrame();

			for (auto control : activeControls)
			{
				handler.ProcessBinding(device, InputControlType::DIGITAL, control);
			}

			lifecycle.PreUpdate();
		}
	};
}


TEST_SUITE_BEGIN("InputActionResolver");


TEST_CASE("Simple bool action starts on press")
{
	ResolverFixture fx;

	fx.device.SetButton(JUMP_KEY, 1.0f);
	fx.Frame({ JUMP_KEY });

	auto view = fx.resolver.GetActionState(USER, "Jump");

	REQUIRE(view.has_value());
	CHECK(view->IsStarted());
	CHECK(view->ReadValue<bool>() == true);
}


TEST_CASE("Held action transitions to performed, release cancels")
{
	ResolverFixture fx;

	fx.device.SetButton(JUMP_KEY, 1.0f);
	fx.Frame({ JUMP_KEY });
	fx.Frame({ JUMP_KEY });

	auto view = fx.resolver.GetActionState(USER, "Jump");
	REQUIRE(view.has_value());
	CHECK(view->IsPerformed());

	fx.device.SetButton(JUMP_KEY, 0.0f);
	fx.Frame({ JUMP_KEY });

	view = fx.resolver.GetActionState(USER, "Jump");
	REQUIRE(view.has_value());
	CHECK(view->IsCancelled());
}


TEST_CASE("Idle action stays waiting")
{
	ResolverFixture fx;

	fx.Frame({});

	auto view = fx.resolver.GetActionState(USER, "Jump");

	REQUIRE(view.has_value());
	CHECK(view->IsWaiting());
	CHECK(view->ReadValue<bool>() == false);
}


TEST_CASE("Composite 1D resolves positive minus negative")
{
	ResolverFixture fx;

	fx.device.SetButton(RIGHT_KEY, 1.0f);
	fx.Frame({ RIGHT_KEY });

	auto view = fx.resolver.GetActionState(USER, "Move");

	REQUIRE(view.has_value());
	CHECK(view->ReadValue<float>() == doctest::Approx(1.0f));

	fx.device.SetButton(RIGHT_KEY, 0.0f);
	fx.device.SetButton(LEFT_KEY, 1.0f);
	fx.Frame({ LEFT_KEY });

	view = fx.resolver.GetActionState(USER, "Move");

	REQUIRE(view.has_value());
	CHECK(view->ReadValue<float>() == doctest::Approx(-1.0f));
}


TEST_CASE("Composite 2D resolves a direction vector")
{
	ResolverFixture fx;

	fx.device.SetButton(RIGHT_KEY, 1.0f);
	fx.device.SetButton(UP_KEY, 1.0f);
	fx.Frame({ RIGHT_KEY, UP_KEY });

	auto view = fx.resolver.GetActionState(USER, "Look");

	REQUIRE(view.has_value());

	auto value = view->ReadValue<Vector2F>();

	CHECK(value.x == doctest::Approx(1.0f));
	CHECK(value.y == doctest::Approx(1.0f));
}


TEST_CASE("Disabled action maps ignore bindings")
{
	ResolverFixture fx;

	fx.resolver.DisableActionMap(USER, "Default");

	fx.device.SetButton(JUMP_KEY, 1.0f);
	fx.Frame({ JUMP_KEY });

	auto view = fx.resolver.GetActionState(USER, "Jump");

	REQUIRE(view.has_value());
	CHECK(view->IsWaiting());
}


TEST_CASE("SwitchToActionMap enables the target and disables the rest")
{
	ResolverFixture fx;

	CHECK(fx.resolver.SwitchToActionMap(USER, "Default") == true);

	fx.device.SetButton(JUMP_KEY, 1.0f);
	fx.Frame({ JUMP_KEY });

	auto view = fx.resolver.GetActionState(USER, "Jump");

	REQUIRE(view.has_value());
	CHECK(view->IsStarted());
}


TEST_CASE("Unknown users and maps are rejected")
{
	ResolverFixture fx;

	CHECK(fx.resolver.EnableActionMap(99, "Default") == false);
	CHECK(fx.resolver.EnableActionMap(USER, "Nope") == false);
	CHECK_FALSE(fx.resolver.GetActionState(99, "Jump").has_value());
}


TEST_CASE("Events from unpaired devices are ignored")
{
	ResolverFixture fx;

	fx.device.id = 55; // not paired to anyone

	fx.device.SetButton(JUMP_KEY, 1.0f);
	fx.Frame({ JUMP_KEY });

	auto view = fx.resolver.GetActionState(USER, "Jump");

	REQUIRE(view.has_value());
	CHECK(view->IsWaiting());
}


TEST_CASE("Bindings for other device types are ignored")
{
	ResolverFixture fx;

	fx.device.type = InputDeviceType::CONTROLLER;

	fx.device.SetButton(JUMP_KEY, 1.0f);
	fx.Frame({ JUMP_KEY });

	auto view = fx.resolver.GetActionState(USER, "Jump");

	REQUIRE(view.has_value());
	CHECK(view->IsWaiting());
}


TEST_CASE("RemoveUser drops the user's runtime state")
{
	ResolverFixture fx;

	fx.resolver.RemoveUser(USER);

	CHECK_FALSE(fx.resolver.GetActionState(USER, "Jump").has_value());
	CHECK(fx.resolver.EnableActionMap(USER, "Default") == false);
}


TEST_CASE("Registering with an invalid listener handle is rejected")
{
	ResolverFixture fx;

	auto id = fx.resolver.RegisterAction(USER, "Default", "Jump", DF2D::Engine::ComponentHandleBase{}, [](const InputActionView&) {});

	CHECK(id == -1);
}


TEST_SUITE_END();