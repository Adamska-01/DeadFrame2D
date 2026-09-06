#include "Core/Context/Systems/UI/Context/UIContext.h"
#include "Engine/ECS/Component/UI/ScrollView.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	ScrollView::ScrollView()
	{
	}


	UIElementType ScrollView::GetElementType() const
	{
		return UIElementType::SCROLL_PANEL;
	}

	void ScrollView::OnElementCreated()
	{
		ApplyOverflow();
	}

	void ScrollView::HandleUIEvent(UIEventType eventType, const UIEventPayload& payload)
	{
		if (eventType != UIEventType::SCROLL)
			return;

		OnScroll.Broadcast(payload.offset);
	}


	void ScrollView::SetHorizontal(bool enabled)
	{
		if (enabled == horizontal)
			return;

		horizontal = enabled;

		ApplyOverflow();
	}

	void ScrollView::SetVertical(bool enabled)
	{
		if (enabled == vertical)
			return;

		vertical = enabled;

		ApplyOverflow();
	}

	void ScrollView::SetScrollPosition(Vector2F position)
	{
		element.AsScrollable().SetScrollOffset(position);
	}


	bool ScrollView::IsHorizontal() const
	{
		return horizontal;
	}

	bool ScrollView::IsVertical() const
	{
		return vertical;
	}

	Vector2F ScrollView::GetScrollPosition() const
	{
		return element.AsScrollable().GetScrollOffset();
	}

	Vector2F ScrollView::GetContentSize() const
	{
		return element.AsScrollable().GetScrollSize();
	}


	void ScrollView::ApplyOverflow()
	{
		// "auto" rather than "scroll": a bar that is only there when there is something to scroll is
		// what every other engine's scroll view does, and it keeps an under-filled view looking plain.
		SetStyle(UIStyleProperty::OVERFLOW_X, horizontal ? "auto" : "hidden");
		SetStyle(UIStyleProperty::OVERFLOW_Y, vertical ? "auto" : "hidden");
	}
}