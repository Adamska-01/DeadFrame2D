#include "Engine/ECS/Component/UI/ScrollView.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	/** @brief Exposes the protected event hook so the widget can be driven without a live backend. */
	struct TestScrollView : ScrollView
	{
		TYPE_INFO(TestScrollView, ScrollView);


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


	ComponentHandle<TestScrollView> MakeScrollView(std::shared_ptr<ComponentBucket>& bucket)
	{
		if (bucket == nullptr)
		{
			bucket = std::make_shared<ComponentBucket>();
		}

		return bucket->AddComponent<TestScrollView>(ObjectHandle<GameObject>{});
	}
}


TEST_SUITE_BEGIN("ScrollView");


TEST_CASE("A scroll view asks for a scroll panel and scrolls down but not across")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto view = MakeScrollView(bucket);

	CHECK(view->ElementType() == UIElementType::SCROLL_PANEL);
	CHECK(view->IsVertical() == true);
	CHECK(view->IsHorizontal() == false);
}


TEST_CASE("Scrolling reports where the content ended up")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto view = MakeScrollView(bucket);

	auto reported = Vector2F::Zero;
	auto reports = 0;

	view->OnScroll.AddLambda([&](Vector2F offset) { reported = offset; reports++; });

	view->Deliver(UIEventType::SCROLL, UIEventPayload{ .offset = Vector2F(4.0f, 32.0f) });

	CHECK(reports == 1);
	CHECK(reported.x == doctest::Approx(4.0f));
	CHECK(reported.y == doctest::Approx(32.0f));
}


TEST_CASE("Events that are not scrolling leave the scroll delegate alone")
{
	std::shared_ptr<ComponentBucket> bucket;
	auto view = MakeScrollView(bucket);

	auto reports = 0;
	view->OnScroll.AddLambda([&reports](Vector2F) { reports++; });

	view->Deliver(UIEventType::CLICK, UIEventPayload{});
	view->Deliver(UIEventType::VALUE_CHANGED, UIEventPayload{});

	CHECK(reports == 0);
}


TEST_SUITE_END();