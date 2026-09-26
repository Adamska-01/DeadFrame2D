#include "Engine/ECS/Component/UI/Slider.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <doctest.h>
#include <memory>
#include <vector>


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


TEST_CASE("Reaching a slider does not hand it its own axis; activating it does")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	slider->Deliver(UIEventType::FOCUS_GAINED, UIEventPayload{});

	REQUIRE(slider->IsFocused());
	REQUIRE_FALSE(slider->IsActivated());

	// Focused only: the axis the slider would use is held, so navigating onto it leaves it untouched.
	CHECK(slider->ResolveNavigation(UINavigationDirection::LEFT) == UINavigationResponse::BLOCK);
	CHECK(slider->ResolveNavigation(UINavigationDirection::RIGHT) == UINavigationResponse::BLOCK);

	// The other axis still moves focus away, the way it does from any other widget.
	CHECK(slider->ResolveNavigation(UINavigationDirection::UP) == UINavigationResponse::MOVE_FOCUS);

	slider->Activate();

	REQUIRE(slider->IsActivated());

	CHECK(slider->ResolveNavigation(UINavigationDirection::LEFT) == UINavigationResponse::CONSUME);
	CHECK(slider->ResolveNavigation(UINavigationDirection::UP) == UINavigationResponse::BLOCK);
}


TEST_CASE("A vertical slider claims the vertical axis instead")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	slider->SetVertical(true);
	slider->Activate();

	CHECK(slider->ResolveNavigation(UINavigationDirection::UP) == UINavigationResponse::CONSUME);
	CHECK(slider->ResolveNavigation(UINavigationDirection::LEFT) == UINavigationResponse::BLOCK);
}


TEST_CASE("Losing focus drops the activation with it")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	auto activations = std::vector<bool>();
	slider->OnActivationChanged.AddLambda([&activations](bool isActivated) { activations.push_back(isActivated); });

	slider->Deliver(UIEventType::FOCUS_GAINED, UIEventPayload{});
	slider->Activate();

	// Interacting with another widget takes focus away from this one, which must take the selection
	// with it: two sliders must never both look selected.
	slider->Deliver(UIEventType::FOCUS_LOST, UIEventPayload{});

	CHECK_FALSE(slider->IsActivated());
	CHECK_FALSE(slider->IsFocused());

	REQUIRE(activations.size() == 2);
	CHECK(activations[0] == true);
	CHECK(activations[1] == false);
}


TEST_CASE("Clicking a slider selects it outright")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	slider->Deliver(UIEventType::CLICK, UIEventPayload{});

	CHECK(slider->IsActivated());
}


TEST_CASE("A slider that is not interactable cannot be selected")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto slider = MakeSlider(bucket);

	slider->SetInteractable(false);
	slider->Activate();

	CHECK_FALSE(slider->IsActivated());
}


TEST_SUITE_END();