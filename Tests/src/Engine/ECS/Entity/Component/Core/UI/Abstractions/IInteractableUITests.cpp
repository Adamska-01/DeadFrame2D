#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	/** @brief Exposes the protected event hook so the gate can be exercised without a live backend. */
	struct TestInteractable : IInteractableUI
	{
		TYPE_INFO(TestInteractable, IInteractableUI);


	public:
		int interactionCount = 0;


		void Deliver(UIEventType eventType)
		{
			HandleUIEvent(eventType, UIEventPayload{});
		}


	protected:
		void OnInteraction(UIEventType, const UIEventPayload&) override
		{
			interactionCount++;
		}
	};


	ComponentHandle<TestInteractable> MakeInteractable(std::shared_ptr<ComponentBucket>& bucket)
	{
		if (bucket == nullptr)
		{
			bucket = std::make_shared<ComponentBucket>();
		}

		return bucket->AddComponent<TestInteractable>(ObjectHandle<GameObject>{});
	}
}


TEST_SUITE_BEGIN("IInteractableUI");


TEST_CASE("An interactable widget receives pointer events and passes them on")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto widget = MakeInteractable(bucket);

	auto entered = 0;
	widget->OnPointerEnter.AddLambda([&entered]() { entered++; });

	widget->Deliver(UIEventType::POINTER_ENTER);

	CHECK(entered == 1);
	CHECK(widget->interactionCount == 1);
}


TEST_CASE("A widget that is not interactable takes part in no interaction at all")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto widget = MakeInteractable(bucket);

	auto entered = 0;
	auto clicked = 0;

	widget->OnPointerEnter.AddLambda([&entered]() { entered++; });

	widget->SetInteractable(false);

	// Not being hit-tested already stops the pointer reaching it; this covers every other route an
	// event could arrive by, such as focus or keyboard activation.
	widget->Deliver(UIEventType::POINTER_ENTER);
	widget->Deliver(UIEventType::CLICK);

	CHECK(entered == 0);
	CHECK(clicked == 0);
	CHECK(widget->interactionCount == 0);
}


TEST_CASE("Making a widget interactable again restores its events")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto widget = MakeInteractable(bucket);

	widget->SetInteractable(false);
	widget->Deliver(UIEventType::CLICK);

	widget->SetInteractable(true);
	widget->Deliver(UIEventType::CLICK);

	CHECK(widget->interactionCount == 1);
	CHECK(widget->IsInteractable());
}


TEST_CASE("Activation is a pointer-level event every interactable reports")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto widget = MakeInteractable(bucket);

	auto clicked = 0;
	widget->OnClick.AddLambda([&clicked]() { clicked++; });

	widget->Deliver(UIEventType::CLICK);

	// Broadcast by the base rather than by each widget: a click means the same thing to a Button, a
	// Toggle and a Slider, unlike VALUE_CHANGED whose payload only its own widget can interpret.
	CHECK(clicked == 1);
}


TEST_CASE("A widget that is not interactable does not report activation")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto widget = MakeInteractable(bucket);

	auto clicked = 0;
	widget->OnClick.AddLambda([&clicked]() { clicked++; });

	widget->SetInteractable(false);
	widget->Deliver(UIEventType::CLICK);

	CHECK(clicked == 0);
}


TEST_SUITE_END();