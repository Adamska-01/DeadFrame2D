#include <doctest.h>
#include "Constants/Input/DefaultDeviceIDs.h"
#include "Core/Context/Systems/Input/User/InputUser.h"
#include "Core/Context/Systems/Input/User/InputUserManager.h"
#include <vector>


using namespace DF2D::Constants;
using namespace DF2D::Core;
using namespace DF2D::Data;


TEST_SUITE_BEGIN("InputUserManager");


TEST_CASE("CreateUser assigns ids, names and auto-pairs keyboard and mouse to the first user")
{
	InputUserManager manager;

	auto* first = manager.CreateUser("PlayerOne");

	REQUIRE(first != nullptr);
	CHECK(first->Name() == "PlayerOne");
	CHECK(first->IsDevicePaired(DefaultDeviceIDs::KEYBOARD));
	CHECK(first->IsDevicePaired(DefaultDeviceIDs::MOUSE));

	auto* second = manager.CreateUser("PlayerTwo");

	CHECK(second->ID() != first->ID());
	CHECK_FALSE(second->IsDevicePaired(DefaultDeviceIDs::KEYBOARD));
}


TEST_CASE("GetUserIDFromPairedDevice resolves pairings")
{
	InputUserManager manager;

	auto* user = manager.CreateUser();

	auto resolved = manager.GetUserIDFromPairedDevice(DefaultDeviceIDs::KEYBOARD);

	REQUIRE(resolved.has_value());
	CHECK(*resolved == user->ID());

	CHECK_FALSE(manager.GetUserIDFromPairedDevice(42).has_value());
}


TEST_CASE("PairDeviceToUser and UnpairDevice update pairings")
{
	InputUserManager manager;

	auto* user = manager.CreateUser();

	manager.PairDeviceToUser(user, 5);

	CHECK(user->IsDevicePaired(5));

	manager.UnpairDevice(user, 5);

	CHECK_FALSE(user->IsDevicePaired(5));
	CHECK_FALSE(manager.GetUserIDFromPairedDevice(5).has_value());
}


TEST_CASE("DestroyUser removes the user and all its pairings")
{
	InputUserManager manager;

	auto* user = manager.CreateUser();
	auto userID = user->ID();

	manager.PairDeviceToUser(user, 5);

	manager.DestroyUser(userID);

	CHECK(manager.GetUser(userID) == nullptr);
	CHECK_FALSE(manager.GetUserIDFromPairedDevice(DefaultDeviceIDs::KEYBOARD).has_value());
	CHECK_FALSE(manager.GetUserIDFromPairedDevice(5).has_value());
}


TEST_CASE("Lifecycle hooks fire on create and destroy")
{
	std::vector<InputUserID> created;
	std::vector<InputUserID> destroyed;

	InputUserManager manager(
		[&](InputUserID id) { created.push_back(id); },
		[&](InputUserID id) { destroyed.push_back(id); });

	auto* user = manager.CreateUser();

	REQUIRE(created.size() == 1);
	CHECK(created[0] == user->ID());

	manager.DestroyUser(user->ID());

	REQUIRE(destroyed.size() == 1);
	CHECK(destroyed[0] == created[0]);
}


TEST_CASE("GetAllUsers returns every live user")
{
	InputUserManager manager;

	manager.CreateUser("A");
	manager.CreateUser("B");

	CHECK(manager.GetAllUsers().size() == 2);
}


TEST_CASE("Destroying an unknown user is a safe no-op")
{
	InputUserManager manager;

	manager.DestroyUser(1234);

	CHECK(manager.GetAllUsers().empty());
}


TEST_SUITE_END();