#include "Engine/Components/UI/Canvas.h"


namespace DeadFrame2D::Engine
{
	Canvas::Canvas()
		: sortOrder(0)
	{
	}

	int Canvas::GetSortOrder() const
	{
		return sortOrder;
	}

	void Canvas::SetSortOrder(int value)
	{
		sortOrder = value;
	}
}