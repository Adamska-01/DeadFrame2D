#include "Utilities/Debugging/Guards.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Utilities;


TEST_SUITE_BEGIN("Guards");


TEST_CASE("AgainstNull throws for a null raw pointer")
{
	int* ptr = nullptr;

	CHECK_THROWS_AS(Guard::AgainstNull(ptr, "ptr"), std::invalid_argument);
}


TEST_CASE("AgainstNull throws for a null shared_ptr")
{
	std::shared_ptr<int> ptr = nullptr;

	CHECK_THROWS_AS(Guard::AgainstNull(ptr, "ptr"), std::invalid_argument);
}


TEST_CASE("AgainstNull does not throw for a non-null pointer")
{
	auto value = 5;
	auto* ptr = &value;

	CHECK_NOTHROW(Guard::AgainstNull(ptr, "ptr"));
}


TEST_CASE("AgainstNull's exception message names the variable")
{
	int* ptr = nullptr;

	try
	{
		Guard::AgainstNull(ptr, "myPointer");
		FAIL("Expected std::invalid_argument to be thrown");
	}
	catch (const std::invalid_argument& e)
	{
		CHECK(std::string(e.what()).find("myPointer") != std::string::npos);
	}
}


TEST_CASE("AgainstNullAssignment returns the same non-null pointer")
{
	auto value = 5;
	auto* ptr = &value;

	auto result = Guard::AgainstNullAssignment(ptr, "ptr");

	CHECK(result == ptr);
}


TEST_CASE("AgainstNullAssignment throws for a null pointer")
{
	int* ptr = nullptr;

	CHECK_THROWS_AS(Guard::AgainstNullAssignment(ptr, "ptr"), std::invalid_argument);
}


TEST_SUITE_END();