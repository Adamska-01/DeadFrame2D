#include "Utilities/Helpers/Events/EventHelpers.h"
#include <doctest.h>


using namespace DF2D::Utilities::EventHelpers;


namespace
{
	class Recorder
	{
	public:
		int lastValue = 0;

		int callCount = 0;


		void Record(int value)
		{
			lastValue = value;
			++callCount;
		}

		int Add(int a, int b)
		{
			++callCount;

			return a + b;
		}
	};
}


TEST_SUITE_BEGIN("EventHelpers");


TEST_CASE("BindFunction invokes the bound member function on the given instance")
{
	auto recorder = Recorder();
	auto bound = BindFunction(&recorder, &Recorder::Record);

	bound(42);

	CHECK(recorder.lastValue == 42);
	CHECK(recorder.callCount == 1);
}


TEST_CASE("BindFunction forwards multiple arguments and returns the result")
{
	auto recorder = Recorder();
	auto bound = BindFunction(&recorder, &Recorder::Add);

	auto result = bound(2, 3);

	CHECK(result == 5);
}


TEST_CASE("BindFunction is independent per bound instance")
{
	auto a = Recorder();
	auto b = Recorder();

	auto boundA = BindFunction(&a, &Recorder::Record);
	auto boundB = BindFunction(&b, &Recorder::Record);

	boundA(1);
	boundB(2);

	CHECK(a.lastValue == 1);
	CHECK(b.lastValue == 2);
}


TEST_SUITE_END();