#include <doctest.h>
#include "Core/Services/Events/EventManager.h"
#include "Mocks/Services/Events/MockEventSource.h"
#include "Mocks/Services/Events/MockSystemEventSink.h"


using namespace DF2D::Core;
using namespace DF2D::Data;


static std::unique_ptr<EventManager> MakeManager(MockEventSource*& outSource)
{
	auto source = std::make_unique<MockEventSource>();
	outSource = source.get();

	return std::make_unique<EventManager>(std::move(source));
}


TEST_SUITE_BEGIN("EventManager");


TEST_CASE("ProcessEvents returns nullopt when the source is empty")
{
	MockEventSource* source = nullptr;
	auto manager = MakeManager(source);

	auto result = manager->ProcessEvents();

	CHECK_FALSE(result.has_value());
	CHECK(source->pollCount == 1);
}


TEST_CASE("QuitEvent returns an exit code")
{
	MockEventSource* source = nullptr;
	auto manager = MakeManager(source);

	source->events.push_back(QuitEvent{});

	auto result = manager->ProcessEvents();

	REQUIRE(result.has_value());
	CHECK(*result == 0);
}


TEST_CASE("Non-quit events are forwarded to registered sinks in order")
{
	MockEventSource* source = nullptr;
	auto manager = MakeManager(source);

	MockSystemEventSink sink;
	manager->AddSink(&sink);

	source->events.push_back(KeyEvent{ DF2D::Models::KeyboardKeyCode::A, true });
	source->events.push_back(MouseWheelEvent{ { 0.0f, 1.0f } });

	auto result = manager->ProcessEvents();

	CHECK_FALSE(result.has_value());
	REQUIRE(sink.receivedEvents.size() == 2);
	CHECK(std::holds_alternative<KeyEvent>(sink.receivedEvents[0]));
	CHECK(std::holds_alternative<MouseWheelEvent>(sink.receivedEvents[1]));
}


TEST_CASE("Events before a quit are still delivered")
{
	MockEventSource* source = nullptr;
	auto manager = MakeManager(source);

	MockSystemEventSink sink;
	manager->AddSink(&sink);

	source->events.push_back(KeyEvent{ DF2D::Models::KeyboardKeyCode::A, true });
	source->events.push_back(QuitEvent{});

	auto result = manager->ProcessEvents();

	REQUIRE(result.has_value());
	CHECK(sink.receivedEvents.size() == 1);
}


TEST_CASE("Multiple sinks all receive each event")
{
	MockEventSource* source = nullptr;
	auto manager = MakeManager(source);

	MockSystemEventSink first, second;
	manager->AddSink(&first);
	manager->AddSink(&second);

	source->events.push_back(KeyEvent{ DF2D::Models::KeyboardKeyCode::A, true });

	manager->ProcessEvents();

	CHECK(first.receivedEvents.size() == 1);
	CHECK(second.receivedEvents.size() == 1);
}


TEST_CASE("Removed sinks stop receiving events")
{
	MockEventSource* source = nullptr;
	auto manager = MakeManager(source);

	MockSystemEventSink sink;
	manager->AddSink(&sink);
	manager->RemoveSink(&sink);

	source->events.push_back(KeyEvent{ DF2D::Models::KeyboardKeyCode::A, true });

	manager->ProcessEvents();

	CHECK(sink.receivedEvents.empty());
}


TEST_CASE("Null sinks are ignored")
{
	MockEventSource* source = nullptr;
	auto manager = MakeManager(source);

	manager->AddSink(nullptr);

	source->events.push_back(KeyEvent{ DF2D::Models::KeyboardKeyCode::A, true });

	CHECK_FALSE(manager->ProcessEvents().has_value());
}


TEST_CASE("RequestQuit pushes a quit through the source")
{
	MockEventSource* source = nullptr;
	auto manager = MakeManager(source);

	manager->RequestQuit();

	CHECK(source->pushQuitCount == 1);

	auto result = manager->ProcessEvents();

	REQUIRE(result.has_value());
	CHECK(*result == 0);
}


TEST_CASE("Every sink receives every event")
{
	MockEventSource* source = nullptr;
	auto manager = MakeManager(source);

	auto first = MockSystemEventSink();
	auto second = MockSystemEventSink();

	manager->AddSink(&first);
	manager->AddSink(&second);

	source->events.push_back(KeyEvent{});

	manager->ProcessEvents();

	// Sinks cannot consume, so registration order carries no meaning and no sink can starve another.
	// The input devices depend on this: skipping one would leave its recorded state lying.
	CHECK(first.receivedEvents.size() == 1);
	CHECK(second.receivedEvents.size() == 1);
}


TEST_SUITE_END();