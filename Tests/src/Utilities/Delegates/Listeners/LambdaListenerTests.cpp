#include "Utilities/Delegates/Listeners/LambdaListener.h"
#include <doctest.h>


using namespace DF2D::Utilities;


TEST_SUITE_BEGIN("LambdaListener");


TEST_CASE("Invoke calls the wrapped lambda with the forwarded argument")
{
	auto received = 0;
	auto listener = LambdaListener<int>(std::function<void(int)>([&](int value) { received = value; }));

	listener.Invoke(9);

	CHECK(received == 9);
}


TEST_CASE("IsExpired is always false, by design")
{
	auto listener = LambdaListener<>(std::function<void()>([]() {}));

	CHECK_FALSE(listener.IsExpired());
}


TEST_CASE("MatchesIdentity is always false, by design (a lambda has no owner)")
{
	auto listener = LambdaListener<>(std::function<void()>([]() {}));
	auto someObject = 5;

	CHECK_FALSE(listener.Matches<int>(someObject));
}


TEST_SUITE_END();