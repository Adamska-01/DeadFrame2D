#include "Utilities/Delegates/Listeners/RawListener.h"
#include <doctest.h>


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

	class OtherOwner
	{
	public:
		void OnEvent(int) {}
	};
}


TEST_SUITE_BEGIN("RawListener");


TEST_CASE("Invoke calls the bound member function on the owner")
{
	auto owner = Owner();
	auto listener = RawListener<Owner, int>(&owner, &Owner::OnEvent);

	listener.Invoke(42);

	CHECK(owner.lastValue == 42);
}


TEST_CASE("IsExpired is false while the owner pointer is non-null")
{
	auto owner = Owner();
	auto listener = RawListener<Owner, int>(&owner, &Owner::OnEvent);

	CHECK_FALSE(listener.IsExpired());
}


TEST_CASE("IsExpired only reflects a manually-nulled owner, not real object destruction")
{
	// Documented limitation (RawListener.h): IsExpired() only checks `owner == nullptr`.
	// It cannot detect a destroyed-but-still-non-null owner - that unsafety is the whole
	// point of raw listeners. This pins the check it DOES perform, not a fix.
	auto owner = Owner();
	auto listener = RawListener<Owner, int>(&owner, &Owner::OnEvent);

	CHECK_FALSE(listener.IsExpired());

	listener.owner = nullptr;

	CHECK(listener.IsExpired());
}


TEST_CASE("MatchesIdentity matches the same owner and type, rejects a different pointer")
{
	auto owner = Owner();
	auto other = Owner();
	auto listener = RawListener<Owner, int>(&owner, &Owner::OnEvent);

	CHECK(listener.Matches<Owner>(owner));
	CHECK_FALSE(listener.Matches<Owner>(other));
}


TEST_CASE("MatchesIdentity rejects a mismatched owner type")
{
	auto owner = Owner();
	auto listener = RawListener<Owner, int>(&owner, &Owner::OnEvent);
	auto otherOwner = OtherOwner();

	CHECK_FALSE(listener.Matches<OtherOwner>(otherOwner));
}


TEST_SUITE_END();