#include "Utilities/Delegates/MulticastDelegate.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Utilities;


namespace
{
	class Counter
	{
	public:
		int calls = 0;


		void Increment()
		{
			++calls;
		}
	};
}


TEST_SUITE_BEGIN("MulticastDelegate");


TEST_CASE("AddRaw registers a listener that is invoked on Broadcast")
{
	MulticastDelegate<> delegate;
	auto owner = Counter();

	delegate.AddRaw(&owner, &Counter::Increment);
	delegate.Broadcast();

	CHECK(owner.calls == 1);
}

TEST_CASE("AddShared registers a listener that is invoked on Broadcast")
{
	MulticastDelegate<> delegate;
	auto owner = std::make_shared<Counter>();

	delegate.AddShared(owner, &Counter::Increment);
	delegate.Broadcast();

	CHECK(owner->calls == 1);
}

TEST_CASE("AddLambda registers a listener that is invoked on Broadcast")
{
	MulticastDelegate<> delegate;
	auto calls = 0;

	delegate.AddLambda([&]() { ++calls; });
	delegate.Broadcast();

	CHECK(calls == 1);
}

TEST_CASE("Broadcast forwards arguments to every listener")
{
	MulticastDelegate<int> delegate;
	auto received = 0;

	delegate.AddLambda([&](int value) { received = value; });
	delegate.Broadcast(7);

	CHECK(received == 7);
}


TEST_CASE("Count reflects only non-expired listeners")
{
	MulticastDelegate<> delegate;
	auto owner = std::make_shared<Counter>();

	delegate.AddShared(owner, &Counter::Increment);
	delegate.AddLambda([]() {});

	CHECK(delegate.Count() == 2);

	owner.reset();

	CHECK(delegate.Count() == 1);
	CHECK_FALSE(delegate.IsEmpty());
}

TEST_CASE("Broadcast prunes expired listeners before invoking the rest")
{
	MulticastDelegate<> delegate;
	auto owner = std::make_shared<Counter>();
	auto lambdaCalls = 0;

	delegate.AddShared(owner, &Counter::Increment);
	delegate.AddLambda([&]() { ++lambdaCalls; });

	owner.reset();

	CHECK_NOTHROW(delegate.Broadcast());
	CHECK(lambdaCalls == 1);
	CHECK(delegate.Count() == 1);
}

TEST_CASE("RemoveByID removes exactly the targeted listener")
{
	MulticastDelegate<> delegate;
	auto aCalls = 0;
	auto bCalls = 0;

	auto idA = delegate.AddLambda([&]() { ++aCalls; });
	delegate.AddLambda([&]() { ++bCalls; });

	delegate.RemoveByID(idA);
	delegate.Broadcast();

	CHECK(aCalls == 0);
	CHECK(bCalls == 1);
}

TEST_CASE("RemoveByListener removes by owner identity")
{
	MulticastDelegate<> delegate;
	auto owner = Counter();
	auto other = Counter();

	delegate.AddRaw(&owner, &Counter::Increment);
	delegate.AddRaw(&other, &Counter::Increment);

	delegate.RemoveByListener<Counter>(&other);
	delegate.Broadcast();

	CHECK(owner.calls == 1);
	CHECK(other.calls == 0);
	CHECK(delegate.Count() == 1);
}

TEST_CASE("RemoveByListener<T> is a no-op when no listener matches that type")
{
	MulticastDelegate<> delegate;
	auto owner = Counter();
	auto unrelated = 5;

	delegate.AddRaw(&owner, &Counter::Increment);
	delegate.RemoveByListener<int>(&unrelated);

	delegate.Broadcast();

	CHECK(owner.calls == 1);
}

TEST_CASE("Clear empties the delegate")
{
	MulticastDelegate<> delegate;

	delegate.AddLambda([]() {});
	delegate.Clear();

	CHECK(delegate.IsEmpty());
}


TEST_CASE("A listener adding a new listener from inside Invoke does not fire until the next broadcast")
{
	MulticastDelegate<> delegate;
	auto added = false;
	auto addedCalls = 0;

	delegate.AddLambda([&]()
		{
			if (added)
				return;

			added = true;
			delegate.AddLambda([&]() { ++addedCalls; });
		});

	CHECK_NOTHROW(delegate.Broadcast());
	CHECK(addedCalls == 0);

	delegate.Broadcast();

	CHECK(addedCalls == 1);
}

TEST_CASE("A listener removing itself from inside Invoke still fires for the rest of that broadcast, and is gone on the next")
{
	MulticastDelegate<> delegate;
	auto selfCalls = 0;
	auto otherCalls = 0;
	ListenerID selfID = 0;

	selfID = delegate.AddLambda([&]()
		{
			++selfCalls;
			delegate.RemoveByID(selfID);
		});
	delegate.AddLambda([&]() { ++otherCalls; });

	CHECK_NOTHROW(delegate.Broadcast());
	CHECK(selfCalls == 1);
	CHECK(otherCalls == 1);
	CHECK(delegate.Count() == 1);

	delegate.Broadcast();

	CHECK(selfCalls == 1);
	CHECK(otherCalls == 2);
}

TEST_CASE("A listener calling Clear from inside Invoke completes safely and leaves the delegate empty")
{
	MulticastDelegate<> delegate;
	auto otherCalls = 0;

	delegate.AddLambda([&]() { delegate.Clear(); });
	delegate.AddLambda([&]() { ++otherCalls; });

	CHECK_NOTHROW(delegate.Broadcast());
	CHECK(otherCalls == 1);
	CHECK(delegate.IsEmpty());
}


TEST_SUITE_END();