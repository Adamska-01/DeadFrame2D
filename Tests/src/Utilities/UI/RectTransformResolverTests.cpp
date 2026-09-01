#include "Utilities/UI/RectTransformResolver.h"
#include <algorithm>
#include <doctest.h>
#include <string>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Utilities;


namespace
{
	std::string ValueOf(const std::vector<ResolvedStyleProperty>& resolved, UIStyleProperty property)
	{
		auto it = std::find_if(
			resolved.begin(),
			resolved.end(),
			[property](const ResolvedStyleProperty& entry)
			{
				return entry.property == property;
			});

		return it != resolved.end() ? it->value : std::string();
	}

	/** @brief Parses the leading number out of a "12.5%" or "8px" value. */
	float NumberIn(const std::string& value)
	{
		return value.empty() ? 0.0f : std::stof(value);
	}

	RectTransformProperties Pinned(Vector2F anchor, Vector2F pivot, Vector2F anchoredPosition, Vector2F sizeDelta)
	{
		return RectTransformProperties
		{
			.anchorMin = anchor,
			.anchorMax = anchor,
			.pivot = pivot,
			.anchoredPosition = anchoredPosition,
			.sizeDelta = sizeDelta
		};
	}
}


TEST_SUITE_BEGIN("RectTransformResolver");


TEST_CASE("Every element is absolutely positioned so the layout engine owns the rectangle")
{
	auto resolved = RectTransformResolver::ResolveRectTransform(RectTransformProperties{});

	CHECK(ValueOf(resolved, UIStyleProperty::POSITION) == "absolute");
}


TEST_CASE("A point anchor places the anchor as a percentage of the parent")
{
	auto resolved = RectTransformResolver::ResolveRectTransform(Pinned({ 0.25f, 0.75f }, Vector2F::Zero, Vector2F::Zero, { 100.0f, 50.0f }));

	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::LEFT)) == doctest::Approx(25.000000));
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::TOP)) == doctest::Approx(75.000000));
}


TEST_CASE("A point anchor sizes the element from sizeDelta")
{
	auto resolved = RectTransformResolver::ResolveRectTransform(Pinned({ 0.5f, 0.5f }, Vector2F::Zero, Vector2F::Zero, { 120.0f, 40.0f }));

	CHECK(ValueOf(resolved, UIStyleProperty::WIDTH) == "120px");
	CHECK(ValueOf(resolved, UIStyleProperty::HEIGHT) == "40px");
}


TEST_CASE("A top-left pivot leaves the element hanging off the anchor with no correction")
{
	auto resolved = RectTransformResolver::ResolveRectTransform(Pinned({ 0.5f, 0.5f }, { 0.0f, 0.0f }, Vector2F::Zero, { 100.0f, 60.0f }));

	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_LEFT) == "0px");
	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_TOP) == "0px");
}


TEST_CASE("A centre pivot pulls the element back by half its size so the pivot lands on the anchor")
{
	auto resolved = RectTransformResolver::ResolveRectTransform(Pinned({ 0.5f, 0.5f }, { 0.5f, 0.5f }, Vector2F::Zero, { 100.0f, 60.0f }));

	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_LEFT) == "-50px");
	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_TOP) == "-30px");
}


TEST_CASE("A bottom-right pivot pulls the element back by its full size")
{
	auto resolved = RectTransformResolver::ResolveRectTransform(Pinned({ 1.0f, 1.0f }, { 1.0f, 1.0f }, Vector2F::Zero, { 80.0f, 20.0f }));

	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_LEFT) == "-80px");
	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_TOP) == "-20px");
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::LEFT)) == doctest::Approx(100.000000));
}


TEST_CASE("anchoredPosition offsets the element on top of the pivot correction")
{
	auto resolved = RectTransformResolver::ResolveRectTransform(Pinned({ 0.5f, 0.5f }, { 0.5f, 0.5f }, { 10.0f, -4.0f }, { 100.0f, 60.0f }));

	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_LEFT) == "-40px");
	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_TOP) == "-34px");
}


TEST_CASE("Stretching an axis pins both its edges and lets the size fall out of them")
{
	auto properties = RectTransformProperties
	{
		.anchorMin = { 0.1f, 0.5f },
		.anchorMax = { 0.9f, 0.5f },
		.sizeDelta = { 8.0f, 30.0f }
	};

	auto resolved = RectTransformResolver::ResolveRectTransform(properties);

	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::LEFT)) == doctest::Approx(10.000000));
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::RIGHT)) == doctest::Approx(10.000000));
	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_LEFT) == "8px");
	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_RIGHT) == "8px");

	// A stretched axis must not also carry an explicit size, or the two would fight.
	CHECK(ValueOf(resolved, UIStyleProperty::WIDTH) == "auto");
}


TEST_CASE("Axes are resolved independently, so one can stretch while the other stays pinned")
{
	auto properties = RectTransformProperties
	{
		.anchorMin = { 0.0f, 0.5f },
		.anchorMax = { 1.0f, 0.5f },
		.pivot = { 0.5f, 0.5f },
		.sizeDelta = { 0.0f, 50.0f }
	};

	auto resolved = RectTransformResolver::ResolveRectTransform(properties);

	CHECK(ValueOf(resolved, UIStyleProperty::WIDTH) == "auto");
	CHECK(ValueOf(resolved, UIStyleProperty::HEIGHT) == "50px");
	CHECK(ValueOf(resolved, UIStyleProperty::BOTTOM) == "auto");
}


TEST_CASE("Stretching both axes fills the parent with an inset on every side")
{
	auto properties = RectTransformProperties
	{
		.anchorMin = { 0.0f, 0.0f },
		.anchorMax = { 1.0f, 1.0f },
		.sizeDelta = { 12.0f, 6.0f }
	};

	auto resolved = RectTransformResolver::ResolveRectTransform(properties);

	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::LEFT)) == doctest::Approx(0.000000));
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::RIGHT)) == doctest::Approx(0.000000));
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::TOP)) == doctest::Approx(0.000000));
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::BOTTOM)) == doctest::Approx(0.000000));
	CHECK(ValueOf(resolved, UIStyleProperty::MARGIN_TOP) == "6px");
	CHECK(ValueOf(resolved, UIStyleProperty::WIDTH) == "auto");
	CHECK(ValueOf(resolved, UIStyleProperty::HEIGHT) == "auto");
}


TEST_SUITE_END();