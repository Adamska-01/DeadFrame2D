#include <doctest.h>
#include "Core/Services/Messaging/SystemDialogBox.h"
#include "Mocks/Services/Messaging/MockDialogBackend.h"
#include <stdexcept>


using namespace DF2D::Core;
using namespace DF2D::Data;


static std::unique_ptr<SystemDialogBox> MakeDialogBox(MockDialogBackend*& outBackend)
{
	auto backend = std::make_unique<MockDialogBackend>();
	outBackend = backend.get();

	return std::make_unique<SystemDialogBox>(std::move(backend));
}


TEST_SUITE_BEGIN("SystemDialogBox");


TEST_CASE("A null backend is rejected")
{
	CHECK_THROWS_AS(SystemDialogBox(nullptr), std::invalid_argument);
}


TEST_CASE("ShowDialogBox forwards the config and returns the picked button")
{
	MockDialogBackend* backend = nullptr;
	auto dialogBox = MakeDialogBox(backend);

	backend->resultToReturn = 7;

	auto config = MessageBoxConfig
	{
		.title = "Title",
		.message = "Message",
		.type = MessageBoxType::WARNING,
		.buttons = { { 7, "Ok", MessageBoxButtonFlags::RETURN_KEY_DEFAULT } }
	};

	auto result = dialogBox->ShowDialogBox(config);

	CHECK(result == 7);
	CHECK(backend->showCount == 1);
	CHECK(backend->lastConfig.title == "Title");
	CHECK(backend->lastConfig.message == "Message");
	CHECK(backend->lastConfig.type == MessageBoxType::WARNING);
	REQUIRE(backend->lastConfig.buttons.size() == 1);
	CHECK(backend->lastConfig.buttons[0].text == "Ok");
}


TEST_CASE("ShowInfoBox builds an information box without buttons")
{
	MockDialogBackend* backend = nullptr;
	auto dialogBox = MakeDialogBox(backend);

	dialogBox->ShowInfoBox("Info", "Something happened");

	CHECK(backend->showCount == 1);
	CHECK(backend->lastConfig.title == "Info");
	CHECK(backend->lastConfig.message == "Something happened");
	CHECK(backend->lastConfig.type == MessageBoxType::INFORMATION);
	CHECK(backend->lastConfig.buttons.empty());
}


TEST_CASE("ShowErrorBox builds an error box without buttons")
{
	MockDialogBackend* backend = nullptr;
	auto dialogBox = MakeDialogBox(backend);

	dialogBox->ShowErrorBox("Error", "Something broke");

	CHECK(backend->lastConfig.title == "Error");
	CHECK(backend->lastConfig.message == "Something broke");
	CHECK(backend->lastConfig.type == MessageBoxType::CRITICAL);
	CHECK(backend->lastConfig.buttons.empty());
}


TEST_CASE("ShowConfirmBox builds a warning box with a Yes and a No button")
{
	MockDialogBackend* backend = nullptr;
	auto dialogBox = MakeDialogBox(backend);

	dialogBox->ShowConfirmBox("Quit", "Are you sure?");

	CHECK(backend->lastConfig.title == "Quit");
	CHECK(backend->lastConfig.message == "Are you sure?");
	CHECK(backend->lastConfig.type == MessageBoxType::WARNING);

	REQUIRE(backend->lastConfig.buttons.size() == 2);

	CHECK(backend->lastConfig.buttons[0].id == SystemDialogBox::CONFIRM_YES_ID);
	CHECK(backend->lastConfig.buttons[0].text == "Yes");
	CHECK(backend->lastConfig.buttons[0].flags == MessageBoxButtonFlags::RETURN_KEY_DEFAULT);

	CHECK(backend->lastConfig.buttons[1].id == SystemDialogBox::CONFIRM_NO_ID);
	CHECK(backend->lastConfig.buttons[1].text == "No");
	CHECK(backend->lastConfig.buttons[1].flags == MessageBoxButtonFlags::ESCAPE_KEY_DEFAULT);
}


TEST_CASE("ShowConfirmBox is true only when the Yes button is picked")
{
	MockDialogBackend* backend = nullptr;
	auto dialogBox = MakeDialogBox(backend);

	backend->resultToReturn = SystemDialogBox::CONFIRM_YES_ID;
	CHECK(dialogBox->ShowConfirmBox("Quit", "Are you sure?"));

	backend->resultToReturn = SystemDialogBox::CONFIRM_NO_ID;
	CHECK_FALSE(dialogBox->ShowConfirmBox("Quit", "Are you sure?"));
}


TEST_CASE("A dismissed confirm box is treated as a No")
{
	MockDialogBackend* backend = nullptr;
	auto dialogBox = MakeDialogBox(backend);

	backend->resultToReturn = SystemDialogBox::NO_BUTTON_SELECTED;

	CHECK_FALSE(dialogBox->ShowConfirmBox("Quit", "Are you sure?"));
}


TEST_SUITE_END();