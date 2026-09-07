#include "Utilities/UI/RectTransformResolver.h"
#include <algorithm>
#include <doctest.h>
#include <string>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Utilities;
using namespace DF2D::Utilities;


namespace
{
	std::string ValueOf(const std::vector<RectTransformResolver::ResolvedStyleProperty>& resolved, UIStyleProperty property)
	{
		auto it = std::find_if(
			resolved.begin(),
			resolved.end(),
			[property](const RectTransformResolver::ResolvedStyleProperty& entry)
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


TEST_CASE("A parent-driven element stays in the flow its parent arranges")
{
	auto properties = Pinned(Vector2F(0.5f, 0.5f), Vector2F(0.5f, 0.5f), Vector2F(40.0f, 90.0f), Vector2F(120.0f, 32.0f));

	auto resolved = RectTransformResolver::ResolveRectTransform(properties, LayoutMode::PARENT_DRIVEN);

	// Absolute positioning would take it out of flow, which is exactly where a layout group needs it.
	CHECK(ValueOf(resolved, UIStyleProperty::POSITION) == "relative");

	// The size it asks for survives; where it wanted to sit does not.
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::WIDTH)) == doctest::Approx(120.0f));
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::HEIGHT)) == doctest::Approx(32.0f));

	// And it is a size, not a suggestion: flex shrinks children to fit by default, which would both
	// resize the element behind the author's back and stop a scrolling container ever overflowing.
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::FLEX_SHRINK)) == doctest::Approx(0.0f));
}


TEST_CASE("A parent-driven element clears the placement it set while positioning itself")
{
	auto properties = Pinned(Vector2F(1.0f, 1.0f), Vector2F(1.0f, 1.0f), Vector2F(-24.0f, -24.0f), Vector2F(200.0f, 40.0f));

	auto resolved = RectTransformResolver::ResolveRectTransform(properties, LayoutMode::PARENT_DRIVEN);

	// These are inline properties, so an element reparented into a group still carries whatever it
	// wrote while it was placing itself. Only writing them back off actually removes them.
	CHECK(ValueOf(resolved, UIStyleProperty::LEFT) == "auto");
	CHECK(ValueOf(resolved, UIStyleProperty::RIGHT) == "auto");
	CHECK(ValueOf(resolved, UIStyleProperty::TOP) == "auto");
	CHECK(ValueOf(resolved, UIStyleProperty::BOTTOM) == "auto");

	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::MARGIN_LEFT)) == doctest::Approx(0.0f));
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::MARGIN_TOP)) == doctest::Approx(0.0f));
}


TEST_CASE("A parent-driven element that stretches lets the parent decide that axis")
{
	auto properties = RectTransformProperties
	{
		.anchorMin = Vector2F(0.0f, 0.5f),
		.anchorMax = Vector2F(1.0f, 0.5f),
		.sizeDelta = Vector2F(0.0f, 56.0f)
	};

	auto resolved = RectTransformResolver::ResolveRectTransform(properties, LayoutMode::PARENT_DRIVEN);

	// "As much as the parent gives me" is the parent's to hand out in flow, not a fraction this
	// element can name for itself.
	CHECK(ValueOf(resolved, UIStyleProperty::WIDTH) == "auto");
	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::HEIGHT)) == doctest::Approx(56.0f));
}


TEST_CASE("Placing itself is what an element does unless told otherwise")
{
	auto properties = Pinned(Vector2F(0.5f, 0.5f), Vector2F(0.5f, 0.5f), Vector2F::Zero, Vector2F(100.0f, 100.0f));

	CHECK(ValueOf(RectTransformResolver::ResolveRectTransform(properties), UIStyleProperty::POSITION) == "absolute");
}

TEST_CASE("A layout element takes the size and flex properties off the rect transform")
{
	auto properties = Pinned(Vector2F(0.5f, 0.5f), Vector2F(0.5f, 0.5f), Vector2F::Zero, Vector2F(120.0f, 32.0f));

	auto resolved = RectTransformResolver::ResolveRectTransform(
		properties,
		LayoutMode::PARENT_DRIVEN,
		LayoutSizeSource::LAYOUT_ELEMENT);

	// Two components describing one box, with whichever applied last winning, is the failure this
	// ownership exists to prevent.
	CHECK(ValueOf(resolved, UIStyleProperty::WIDTH).empty());
	CHECK(ValueOf(resolved, UIStyleProperty::HEIGHT).empty());
	CHECK(ValueOf(resolved, UIStyleProperty::FLEX_SHRINK).empty());

	// Position stays this component's job either way.
	CHECK(ValueOf(resolved, UIStyleProperty::POSITION) == "relative");
	CHECK(ValueOf(resolved, UIStyleProperty::LEFT) == "auto");
}


TEST_CASE("Sizing belongs to the rect transform when nothing overrides it")
{
	auto properties = Pinned(Vector2F(0.5f, 0.5f), Vector2F(0.5f, 0.5f), Vector2F::Zero, Vector2F(120.0f, 32.0f));

	auto resolved = RectTransformResolver::ResolveRectTransform(properties, LayoutMode::PARENT_DRIVEN);

	CHECK(NumberIn(ValueOf(resolved, UIStyleProperty::WIDTH)) == doctest::Approx(120.0f));
}


TEST_CASE("An element placing itself is unaffected by who owns its size")
{
	auto properties = Pinned(Vector2F(0.0f, 0.0f), Vector2F(0.0f, 0.0f), Vector2F(16.0f, 16.0f), Vector2F(64.0f, 64.0f));

	auto owned = RectTransformResolver::ResolveRectTransform(
		properties,
		LayoutMode::SELF_POSITIONED,
		LayoutSizeSource::LAYOUT_ELEMENT);

	// There is no group to override, so the anchor model still resolves in full.
	CHECK(ValueOf(owned, UIStyleProperty::POSITION) == "absolute");
	CHECK(NumberIn(ValueOf(owned, UIStyleProperty::WIDTH)) == doctest::Approx(64.0f));
}


TEST_SUITE_END();