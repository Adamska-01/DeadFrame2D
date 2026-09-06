#include "Core/Context/Systems/UI/Elements/UIScrollable.h"
#include "Core/Context/Systems/UI/UIManager.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;


	UIScrollable::UIScrollable(UIManager* manager, UIElementID id)
		: manager(manager),
		id(id)
	{
	}


	bool UIScrollable::IsValid() const
	{
		return manager != nullptr && id != 0;
	}


	Vector2F UIScrollable::GetScrollOffset() const
	{
		return IsValid() ? manager->GetElementScrollOffset(id) : Vector2F::Zero;
	}

	void UIScrollable::SetScrollOffset(Vector2F offset)
	{
		if (IsValid())
		{
			manager->SetElementScrollOffset(id, offset);
		}
	}

	Vector2F UIScrollable::GetScrollSize() const
	{
		return IsValid() ? manager->GetElementScrollSize(id) : Vector2F::Zero;
	}
}