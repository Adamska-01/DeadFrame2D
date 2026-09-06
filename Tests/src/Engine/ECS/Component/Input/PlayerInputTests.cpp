#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Input/User/Abstractions/IInputUsers.h"
#include "Core/Services/Events/Abstractions/ISystemEventSink.h"
#include "Data/Systems/Events/SystemEvent.h"
#include "Engine/ECS/Component/Input/PlayerInput.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Models/Input/ActionMap/InputActionMapBucket.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;
using namespace DF2D::Models;
using namespace DF2D::Utilities;


namespace
{
	InputActionMapBucket MakeBucket()
	{
		auto map = InputActionMap
		{
			.name = "Default",
			.actions =
			{
				InputAction{ .name = "Jump", .valueType = ValueType::BOOL }
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
						.value = static_cast<int>(KeyboardKeyCode::SPACE)
					}
				}
			}
		};

		return InputActionMapBucket{ .actionMaps = { map } };
	}


	struct PlayerInputFixture
	{
		EventDispatcher eventDispatcher;

		Input input{ MakeBucket(), eventDispatcher, nullptr };

		std::shared_ptr<FakeSceneHandleProvider> scene;


		PlayerInputFixture()
		{
			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .input = &input });
		}


		void PressSpace()
		{
			static_cast<ISystemEventSink&>(input).OnSystemEvent(SystemEvent{ KeyEvent{ KeyboardKeyCode::SPACE, true } });
			static_cast<ICoreSystem&>(input).PreUpdate(0.016f);
		}
	};
}


TEST_SUITE_BEGIN("PlayerInput");


TEST_CASE("Init creates an input user")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");

	CHECK(fixture.input.Users()->GetAllUsers().empty());

	playerInput->Init();

	CHECK(fixture.input.Users()->GetAllUsers().size() == 1);
}

TEST_CASE("RegisterAction before Init returns the invalid sentinel and queues the call")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<DummyGameComponent>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");

	auto callCount = 0;
	auto id = playerInput->RegisterAction("Default", "Jump", listener, [&](const InputActionView&) { callCount++; });

	CHECK(id == static_cast<ListenerID>(-1));

	playerInput->Init();
	playerInput->EnableActionMap("Default");
	fixture.PressSpace();

	CHECK(callCount == 1);
}

TEST_CASE("RegisterAction after Init forwards immediately and reports the real listener id")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<DummyGameComponent>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");
	playerInput->Init();
	playerInput->EnableActionMap("Default");

	auto callCount = 0;
	auto id = playerInput->RegisterAction("Default", "Jump", listener, [&](const InputActionView&) { callCount++; });

	CHECK(id != static_cast<ListenerID>(-1));

	fixture.PressSpace();

	CHECK(callCount == 1);
}

TEST_CASE("EnableActionMap before Init queues the call; Init replays it")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<DummyGameComponent>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");

	CHECK_FALSE(playerInput->EnableActionMap("Default"));

	auto callCount = 0;
	playerInput->RegisterAction("Default", "Jump", listener, [&](const InputActionView&) { callCount++; });
	playerInput->Init();
	fixture.PressSpace();

	CHECK(callCount == 1);
}

TEST_CASE("DisableActionMap stops a registered action from firing")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<DummyGameComponent>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");
	playerInput->Init();
	playerInput->EnableActionMap("Default");

	auto callCount = 0;
	playerInput->RegisterAction("Default", "Jump", listener, [&](const InputActionView&) { callCount++; });
	playerInput->DisableActionMap("Default");
	fixture.PressSpace();

	CHECK(callCount == 0);
}

TEST_CASE("DeregisterAction removes only the matching listener")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listenerA = owner->AddComponent<DummyGameComponent>();
	auto listenerB = owner->AddComponent<OtherDummyGameComponent>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");
	playerInput->Init();
	playerInput->EnableActionMap("Default");

	auto callsA = 0, callsB = 0;
	playerInput->RegisterAction("Default", "Jump", listenerA, [&](const InputActionView&) { callsA++; });
	playerInput->RegisterAction("Default", "Jump", listenerB, [&](const InputActionView&) { callsB++; });

	playerInput->DeregisterAction("Default", "Jump", listenerA);
	fixture.PressSpace();

	CHECK(callsA == 0);
	CHECK(callsB == 1);
}

TEST_CASE("DeregisterActionByID removes the specific registration")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<DummyGameComponent>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");
	playerInput->Init();
	playerInput->EnableActionMap("Default");

	auto callCount = 0;
	auto id = playerInput->RegisterAction("Default", "Jump", listener, [&](const InputActionView&) { callCount++; });
	playerInput->DeregisterActionByID("Default", "Jump", id);
	fixture.PressSpace();

	CHECK(callCount == 0);
}

TEST_CASE("DeregisterActionByID before Init is a no-op (no real id can exist yet)")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");

	CHECK_NOTHROW(playerInput->DeregisterActionByID("Default", "Jump", static_cast<ListenerID>(-1)));
}

TEST_CASE("Removing the component deregisters its listeners and destroys its input user")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto listener = owner->AddComponent<DummyGameComponent>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");
	playerInput->Init();
	playerInput->EnableActionMap("Default");

	auto callCount = 0;
	playerInput->RegisterAction("Default", "Jump", listener, [&](const InputActionView&) { callCount++; });

	owner->RemoveComponent(playerInput);

	CHECK(fixture.input.Users()->GetAllUsers().empty());

	fixture.PressSpace();

	CHECK(callCount == 0);
}

TEST_CASE("GetPlayerName returns the constructor-provided name before and after Init")
{
	PlayerInputFixture fixture;
	auto owner = fixture.scene->Create<TestGameObject>();
	auto playerInput = owner->AddComponent<PlayerInput>("P1");

	CHECK(playerInput->GetPlayerName() == "P1");

	playerInput->Init();

	CHECK(playerInput->GetPlayerName() == "P1");
}


TEST_SUITE_END();