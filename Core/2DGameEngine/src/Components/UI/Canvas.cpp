#include "Components/UI/Canvas.h"


Canvas::Canvas()
	: sortOrder(0)
{
}

void Canvas::Init()
{

}

void Canvas::Update(float deltaTime)
{

}

void Canvas::Draw()
{

}

int Canvas::SetSortOrder() const
{
	return sortOrder;
}

void Canvas::SetSortOrder(int value)
{
	sortOrder = value;
}