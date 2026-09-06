#include "Engine/ECS/Component/UI/Toggle.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	/** @brief Exposes the protected event hook so the widget can be driven without a live backend. */
	struct TestToggle : Toggle
	{
		TYPE_INFO(TestToggle, Toggle);


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


	ComponentHandle<TestToggle> MakeToggle(std::shared_ptr<ComponentBucket>& bucket)
	{
		if (bucket == nullptr)
		{
			bucket = std::make_shared<ComponentBucket>();
		}

		return bucket->AddComponent<TestToggle>(ObjectHandle<GameObject>{});
	}
}


TEST_SUITE_BEGIN("Toggle");


TEST_CASE("A toggle asks for a checkbox and starts off")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto toggle = MakeToggle(bucket);

	CHECK(toggle->ElementType() == UIElementType::CHECKBOX);
	CHECK(toggle->IsOn() == false);
}


TEST_CASE("The backend's value decides the state, rather than the state being flipped")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto toggle = MakeToggle(bucket);

	auto reported = 0;
	auto lastState = false;

	toggle->OnValueChanged.AddLambda([&](bool state) { reported++; lastState = state; });

	// A checked control submits its value; an unchecked one submits nothing at all.
	toggle->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .value = "on" });

	CHECK(reported == 1);
	CHECK(lastState == true);
	CHECK(toggle->IsOn() == true);

	toggle->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .value = "" });

	CHECK(reported == 2);
	CHECK(lastState == false);
	CHECK(toggle->IsOn() == false);
}


TEST_CASE("Setting the state reports it once, and the backend's echo does not report it again")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto toggle = MakeToggle(bucket);

	auto reported = 0;
	toggle->OnValueChanged.AddLambda([&reported](bool) { reported++; });

	toggle->SetIsOn(true);

	CHECK(reported == 1);

	// What a real backend sends back after the attribute is written. The state is already stored, so
	// this pass must find nothing new.
	toggle->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .value = "on" });

	CHECK(reported == 1);
	CHECK(toggle->IsOn() == true);
}


TEST_CASE("Setting the state it already holds reports nothing")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto toggle = MakeToggle(bucket);

	auto reported = 0;
	toggle->OnValueChanged.AddLambda([&reported](bool) { reported++; });

	toggle->SetIsOn(false);

	CHECK(reported == 0);
}


TEST_CASE("A toggle that is not interactable ignores value changes")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto toggle = MakeToggle(bucket);

	auto reported = 0;
	toggle->OnValueChanged.AddLambda([&reported](bool) { reported++; });

	toggle->SetInteractable(false);

	toggle->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .value = "on" });

	CHECK(reported == 0);
	CHECK(toggle->IsOn() == false);
}


TEST_SUITE_END();