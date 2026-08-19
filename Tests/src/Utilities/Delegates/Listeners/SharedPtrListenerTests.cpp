#include "Utilities/Delegates/Listeners/SharedPtrListener.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Utilities;


namespace
{
	class Owner
	{
	public:
		int lastValue = 0;


		void OnEvent(int value)
		{
			lastValue = value;
		}
	};
}


TEST_SUITE_BEGIN("SharedPtrListener");


TEST_CASE("Invoke calls through while the shared_ptr is alive")
{
	auto owner = std::make_shared<Owner>();
	auto listener = SharedPtrListener<Owner, int>(owner, &Owner::OnEvent);

	listener.Invoke(7);

	CHECK(owner->lastValue == 7);
}


TEST_CASE("IsExpired becomes true once the owning shared_ptr is destroyed")
{
	auto owner = std::make_shared<Owner>();
	auto listener = SharedPtrListener<Owner, int>(owner, &Owner::OnEvent);

	CHECK_FALSE(listener.IsExpired());

	owner.reset();

	CHECK(listener.IsExpired());
	CHECK_NOTHROW(listener.Invoke(1));
}


TEST_CASE("MatchesIdentity matches the same live object and rejects a different one")
{
	auto owner = std::make_shared<Owner>();
	auto other = std::make_shared<Owner>();
	auto listener = SharedPtrListener<Owner, int>(owner, &Owner::OnEvent);

	CHECK(listener.Matches<std::shared_ptr<Owner>>(owner));
	CHECK_FALSE(listener.Matches<std::shared_ptr<Owner>>(other));
}


TEST_CASE("MatchesIdentity rejects a mismatched identity type")
{
	auto owner = std::make_shared<Owner>();
	auto listener = SharedPtrListener<Owner, int>(owner, &Owner::OnEvent);

	CHECK_FALSE(listener.Matches<Owner>(*owner));
}


TEST_SUITE_END();