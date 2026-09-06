#include "Engine/ECS/Component/UI/Slider.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	/** @brief Exposes the protected event hook so the widget can be driven without a live backend. */
	struct TestSlider : Slider
	{
		TYPE_INFO(TestSlider, Slider);


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


	ComponentHandle<TestSlider> MakeSlider(std::shared_ptr<ComponentBucket>& bucket)
	{
		if (bucket == nullptr)
		{
			bucket = std::make_shared<ComponentBucket>();
		}

		return bucket->AddComponent<TestSlider>(ObjectHandle<GameObject>{});
	}
}


TEST_SUITE_BEGIN("Slider");


TEST_CASE("A slider asks for a range element and starts at zero over zero to one")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	CHECK(slider->ElementType() == UIElementType::RANGE);
	CHECK(slider->GetMinimum() == doctest::Approx(0.0f));
	CHECK(slider->GetMaximum() == doctest::Approx(1.0f));
	CHECK(slider->GetValue() == doctest::Approx(0.0f));
}


TEST_CASE("A value outside the range is clamped into it")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	slider->SetRange(10.0f, 20.0f);

	slider->SetValue(100.0f);
	CHECK(slider->GetValue() == doctest::Approx(20.0f));

	slider->SetValue(-5.0f);
	CHECK(slider->GetValue() == doctest::Approx(10.0f));
}


TEST_CASE("Narrowing the range pulls a value that no longer fits back inside it")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	slider->SetRange(0.0f, 100.0f);
	slider->SetValue(90.0f);

	auto reported = 0.0f;
	slider->OnValueChanged.AddLambda([&reported](float value) { reported = value; });

	slider->SetRange(0.0f, 50.0f);

	CHECK(slider->GetValue() == doctest::Approx(50.0f));
	CHECK(reported == doctest::Approx(50.0f));
}


TEST_CASE("A backwards range is not accepted as one")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	slider->SetRange(10.0f, 2.0f);

	CHECK(slider->GetMinimum() == doctest::Approx(10.0f));
	CHECK(slider->GetMaximum() == doctest::Approx(10.0f));
}


TEST_CASE("Setting the value reports it once, and the backend's echo does not report it again")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	auto reported = 0;
	slider->OnValueChanged.AddLambda([&reported](float) { reported++; });

	slider->SetValue(0.5f);

	CHECK(reported == 1);

	slider->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .numericValue = 0.5f });

	CHECK(reported == 1);
}


TEST_CASE("A value the backend snapped to the step is reported with the number it settled on")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	slider->SetRange(0.0f, 10.0f);
	slider->SetStep(2.0f);

	auto reported = 0.0f;
	auto reports = 0;

	slider->OnValueChanged.AddLambda([&](float value) { reported = value; reports++; });

	slider->SetValue(5.0f);

	CHECK(reports == 1);
	CHECK(reported == doctest::Approx(5.0f));

	// What a stepped backend sends back: the nearest value it can actually hold.
	slider->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .numericValue = 6.0f });

	CHECK(reports == 2);
	CHECK(reported == doctest::Approx(6.0f));
	CHECK(slider->GetValue() == doctest::Approx(6.0f));
}


TEST_CASE("A slider that is not interactable ignores value changes")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	auto reported = 0;
	slider->OnValueChanged.AddLambda([&reported](float) { reported++; });

	slider->SetInteractable(false);

	slider->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{ .numericValue = 0.5f });

	CHECK(reported == 0);
	CHECK(slider->GetValue() == doctest::Approx(0.0f));
}


TEST_SUITE_END();