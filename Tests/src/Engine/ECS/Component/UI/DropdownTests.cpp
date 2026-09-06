#include "Engine/ECS/Component/UI/Dropdown.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	ComponentHandle<Dropdown> MakeDropdown(std::shared_ptr<ComponentBucket>& bucket)
	{
		if (bucket == nullptr)
		{
			bucket = std::make_shared<ComponentBucket>();
		}

		return bucket->AddComponent<Dropdown>(ObjectHandle<GameObject>{});
	}
}


TEST_SUITE_BEGIN("Dropdown");


TEST_CASE("A dropdown starts empty with nothing selected")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto dropdown = MakeDropdown(bucket);

	CHECK(dropdown->GetOptions().empty());
	CHECK(dropdown->GetSelectedIndex() == -1);
	CHECK(dropdown->GetSelectedOption().empty());
}


TEST_CASE("Setting options selects the first one")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto dropdown = MakeDropdown(bucket);

	auto reported = -2;
	dropdown->OnValueChanged.AddLambda([&reported](int index) { reported = index; });

	dropdown->SetOptions({ "Low", "Medium", "High" });

	CHECK(dropdown->GetOptions().size() == 3);
	CHECK(dropdown->GetSelectedIndex() == 0);
	CHECK(dropdown->GetSelectedOption() == "Low");
	CHECK(reported == 0);
}


TEST_CASE("Setting an empty list of options leaves nothing selected")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto dropdown = MakeDropdown(bucket);

	dropdown->SetOptions({ "Low" });
	dropdown->SetOptions({});

	CHECK(dropdown->GetSelectedIndex() == -1);
	CHECK(dropdown->GetSelectedOption().empty());
}


TEST_CASE("An index outside the list clears the selection rather than being kept")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto dropdown = MakeDropdown(bucket);

	dropdown->SetOptions({ "Low", "High" });

	dropdown->SetSelectedIndex(7);

	CHECK(dropdown->GetSelectedIndex() == -1);
}


TEST_CASE("Appending an option returns its index and leaves the selection alone")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto dropdown = MakeDropdown(bucket);

	dropdown->SetOptions({ "Low" });

	auto index = dropdown->AddOption("High");

	CHECK(index == 1);
	CHECK(dropdown->GetOptions().size() == 2);
	CHECK(dropdown->GetSelectedIndex() == 0);
}


TEST_CASE("Clearing the options clears the selection with them")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto dropdown = MakeDropdown(bucket);

	dropdown->SetOptions({ "Low", "High" });

	auto reported = 0;
	dropdown->OnValueChanged.AddLambda([&reported](int index) { reported = index; });

	dropdown->ClearOptions();

	CHECK(dropdown->GetOptions().empty());
	CHECK(dropdown->GetSelectedIndex() == -1);
	CHECK(reported == -1);
}


TEST_SUITE_END();