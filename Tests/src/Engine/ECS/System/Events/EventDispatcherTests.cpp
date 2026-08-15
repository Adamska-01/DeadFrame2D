#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Engine;


namespace
{
	class TestEventA : public DispatchableEvent
	{
	public:
		int value;


		explicit TestEventA(int value = 0)
			: value(value)
		{
		}
	};

	class TestEventB : public DispatchableEvent
	{
	};

	class Listener
	{
	public:
		int received = 0;

		int lastValue = -1;


		void OnEventA(std::shared_ptr<DispatchableEvent> dispatchableEvent)
		{
			++received;

			auto casted = DispatchableEvent::SafeCast<TestEventA>(dispatchableEvent);

			if (casted)
				lastValue = casted->value;
		}
	};
}


TEST_SUITE_BEGIN("EventDispatcher");


TEST_CASE("SendEvent with no registered handler does not crash")
{
	EventDispatcher dispatcher;

	CHECK_NOTHROW(dispatcher.SendEvent(std::make_shared<TestEventA>(1)));
}

TEST_CASE("A registered handler receives the sent event")
{
	EventDispatcher dispatcher;
	Listener listener;

	dispatcher.RegisterEventHandler<TestEventA>(&listener, &Listener::OnEventA);
	dispatcher.SendEvent(std::make_shared<TestEventA>(42));

	CHECK(listener.received == 1);
	CHECK(listener.lastValue == 42);
}

TEST_CASE("Multiple handlers for the same event type all fire")
{
	EventDispatcher dispatcher;
	Listener a;
	Listener b;

	dispatcher.RegisterEventHandler<TestEventA>(&a, &Listener::OnEventA);
	dispatcher.RegisterEventHandler<TestEventA>(&b, &Listener::OnEventA);

	dispatcher.SendEvent(std::make_shared<TestEventA>(7));

	CHECK(a.received == 1);
	CHECK(b.received == 1);
}

TEST_CASE("A handler registered for a different event type is not invoked")
{
	EventDispatcher dispatcher;
	Listener listener;

	dispatcher.RegisterEventHandler<TestEventA>(&listener, &Listener::OnEventA);
	dispatcher.SendEvent(std::make_shared<TestEventB>());

	CHECK(listener.received == 0);
}

TEST_CASE("DeregisterEventHandler stops further delivery")
{
	EventDispatcher dispatcher;
	Listener listener;

	dispatcher.RegisterEventHandler<TestEventA>(&listener, &Listener::OnEventA);
	dispatcher.DeregisterEventHandler<TestEventA>(&listener);

	dispatcher.SendEvent(std::make_shared<TestEventA>(5));

	CHECK(listener.received == 0);
}

TEST_CASE("Deregistering an instance that was never registered is a no-op")
{
	EventDispatcher dispatcher;
	Listener listener;

	CHECK_NOTHROW(dispatcher.DeregisterEventHandler<TestEventA>(&listener));
}

TEST_CASE("Two independent EventDispatcher instances do not share handlers")
{
	EventDispatcher dispatcherA;
	EventDispatcher dispatcherB;
	Listener listener;

	dispatcherA.RegisterEventHandler<TestEventA>(&listener, &Listener::OnEventA);

	dispatcherB.SendEvent(std::make_shared<TestEventA>(99));

	CHECK(listener.received == 0);

	dispatcherA.SendEvent(std::make_shared<TestEventA>(1));

	CHECK(listener.received == 1);
	CHECK(listener.lastValue == 1);
}

TEST_CASE("A shared_ptr-based listener receives the event while alive")
{
	EventDispatcher dispatcher;
	auto listener = std::make_shared<Listener>();

	dispatcher.RegisterEventHandler<TestEventA>(listener, &Listener::OnEventA);
	dispatcher.SendEvent(std::make_shared<TestEventA>(42));

	CHECK(listener->received == 1);
	CHECK(listener->lastValue == 42);
}

TEST_CASE("A shared_ptr-based listener self-expires once the shared_ptr is destroyed")
{
	EventDispatcher dispatcher;

	{
		auto listener = std::make_shared<Listener>();

		dispatcher.RegisterEventHandler<TestEventA>(listener, &Listener::OnEventA);
	}

	// The owning shared_ptr is gone; the dangling weak_ptr must be skipped, not dereferenced.
	CHECK_NOTHROW(dispatcher.SendEvent(std::make_shared<TestEventA>(1)));
}

TEST_CASE("DeregisterEventHandler(shared_ptr) stops further delivery")
{
	EventDispatcher dispatcher;
	auto listener = std::make_shared<Listener>();

	dispatcher.RegisterEventHandler<TestEventA>(listener, &Listener::OnEventA);
	dispatcher.DeregisterEventHandler<TestEventA>(listener);

	dispatcher.SendEvent(std::make_shared<TestEventA>(5));

	CHECK(listener->received == 0);
}

TEST_CASE("A ComponentHandle-based listener receives the event while alive")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();
	auto comp = obj->AddComponent<DummyGameComponent>();

	int received = 0;

	dispatcher.RegisterEventHandler<TestEventA>(comp, [&](std::shared_ptr<DispatchableEvent>) { ++received; });
	dispatcher.SendEvent(std::make_shared<TestEventA>(1));

	CHECK(received == 1);
}

TEST_CASE("A ComponentHandle-based listener self-expires once the component is removed")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();
	auto comp = obj->AddComponent<DummyGameComponent>();

	int received = 0;

	dispatcher.RegisterEventHandler<TestEventA>(comp, [&](std::shared_ptr<DispatchableEvent>) { ++received; });

	obj->RemoveComponent(comp);

	CHECK_NOTHROW(dispatcher.SendEvent(std::make_shared<TestEventA>(1)));
	CHECK(received == 0);
}

TEST_CASE("DeregisterEventHandler(ComponentHandleBase) stops further delivery")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();
	auto comp = obj->AddComponent<DummyGameComponent>();

	int received = 0;

	dispatcher.RegisterEventHandler<TestEventA>(comp, [&](std::shared_ptr<DispatchableEvent>) { ++received; });
	dispatcher.DeregisterEventHandler<TestEventA>(comp);

	dispatcher.SendEvent(std::make_shared<TestEventA>(1));

	CHECK(received == 0);
}

TEST_CASE("DeregisterEventHandler(ObjectHandleBase) stops further delivery")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	int received = 0;

	dispatcher.RegisterEventHandler<TestEventA>(obj, [&](std::shared_ptr<DispatchableEvent>) { ++received; });
	dispatcher.DeregisterEventHandler<TestEventA>(obj);

	dispatcher.SendEvent(std::make_shared<TestEventA>(1));

	CHECK(received == 0);
}

TEST_CASE("Deregistering a ComponentHandle that was never registered is a no-op")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();
	auto comp = obj->AddComponent<DummyGameComponent>();

	CHECK_NOTHROW(dispatcher.DeregisterEventHandler<TestEventA>(comp));
}

TEST_CASE("Deregistering an ObjectHandle that was never registered is a no-op")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	CHECK_NOTHROW(dispatcher.DeregisterEventHandler<TestEventA>(obj));
}

TEST_CASE("Deregistering a shared_ptr listener that was never registered is a no-op")
{
	EventDispatcher dispatcher;
	auto listener = std::make_shared<Listener>();

	CHECK_NOTHROW(dispatcher.DeregisterEventHandler<TestEventA>(listener));
}

TEST_CASE("Deregistering one of several listeners only removes that one")
{
	EventDispatcher dispatcher;
	Listener a;
	Listener b;
	Listener c;

	dispatcher.RegisterEventHandler<TestEventA>(&a, &Listener::OnEventA);
	dispatcher.RegisterEventHandler<TestEventA>(&b, &Listener::OnEventA);
	dispatcher.RegisterEventHandler<TestEventA>(&c, &Listener::OnEventA);

	dispatcher.DeregisterEventHandler<TestEventA>(&b);

	dispatcher.SendEvent(std::make_shared<TestEventA>(1));

	CHECK(a.received == 1);
	CHECK(b.received == 0);
	CHECK(c.received == 1);
}

TEST_CASE("An ObjectHandle-based listener self-expires once the object is destroyed")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	int received = 0;

	dispatcher.RegisterEventHandler<TestEventA>(obj, [&](std::shared_ptr<DispatchableEvent>) { ++received; });
	dispatcher.SendEvent(std::make_shared<TestEventA>(1));

	CHECK(received == 1);

	scene->Destroy(obj.GetIndex());

	dispatcher.SendEvent(std::make_shared<TestEventA>(2));

	CHECK(received == 1);
}


TEST_SUITE_END();