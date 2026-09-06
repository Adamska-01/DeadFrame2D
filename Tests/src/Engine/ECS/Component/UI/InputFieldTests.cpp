#include "Engine/ECS/Component/UI/InputField.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <doctest.h>
#include <memory>
#include <string>


using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	/** @brief Exposes the protected event hook so the widget can be driven without a live backend. */
	struct TestInputField : InputField
	{
		TYPE_INFO(TestInputField, InputField);


	public:
		void Deliver(UIEventType eventType, const UIEventPayload& payload)
		{
			HandleUIEvent(eventType, payload);
		}

		UIElementType ElementType() const
		{
			return GetElementType();
		}
	};


	ComponentHandle<TestInputField> MakeField(std::shared_ptr<ComponentBucket>& bucket)
	{
		if (bucket == nullptr)
		{
			bucket = std::make_shared<ComponentBucket>();
		}

		return bucket->AddComponent<TestInputField>(ObjectHandle<GameObject>{});
	}
}


TEST_SUITE_BEGIN("InputField");


TEST_CASE("A field is a single line until it is told otherwise")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto field = MakeField(bucket);

	CHECK(field->ElementType() == UIElementType::TEXT_INPUT);
	CHECK(field->IsMultiline() == false);

	field->SetMultiline(true);

	CHECK(field->ElementType() == UIElementType::TEXT_AREA);
}


TEST_CASE("Typing updates the value and reports it")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto field = MakeField(bucket);

	auto reported = std::string();
	auto reports = 0;

	field->OnValueChanged.AddLambda([&](const std::string& value) { reported = value; reports++; });

	field->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .value = "Player" });

	CHECK(reports == 1);
	CHECK(reported == "Player");
	CHECK(field->GetText() == "Player");
}


TEST_CASE("Setting the text reports it once, and the backend echo does not report it again")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto field = MakeField(bucket);

	auto reports = 0;
	field->OnValueChanged.AddLambda([&reports](const std::string&) { reports++; });

	field->SetText("Player");

	CHECK(reports == 1);

	field->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .value = "Player" });

	CHECK(reports == 1);
}


TEST_CASE("Committing the value reports the text without reporting a change")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto field = MakeField(bucket);

	auto submitted = std::string();
	auto submissions = 0;
	auto changes = 0;

	field->OnValueChanged.AddLambda([&changes](const std::string&) { changes++; });
	field->OnSubmit.AddLambda([&](const std::string& value) { submitted = value; submissions++; });

	field->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .value = "Player" });
	field->Deliver(UIEventType::SUBMIT, UIEventPayload{ .value = "Player" });

	CHECK(changes == 1);
	CHECK(submissions == 1);
	CHECK(submitted == "Player");
}


TEST_CASE("A field that is not interactable ignores typing")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto field = MakeField(bucket);

	auto reports = 0;
	field->OnValueChanged.AddLambda([&reports](const std::string&) { reports++; });

	field->SetInteractable(false);

	field->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .value = "Player" });

	CHECK(reports == 0);
	CHECK(field->GetText().empty());
}


TEST_SUITE_END();