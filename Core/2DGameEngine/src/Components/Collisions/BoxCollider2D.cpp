#include "Components/Collisions/BoxCollider2D.h"
#include "Tools/Collisions/ColliderVisitor.h"
#include "SubSystems/Renderer.h"


BoxCollider2D::BoxCollider2D(SDL_Rect box, SDL_Rect cropOffset)
	: box(box), cropOffset(cropOffset)
{
}

SDL_Rect BoxCollider2D::GetCollisionBox() const
{
	return box;
}

void BoxCollider2D::SetBuffer(int x, int y, int w, int h)
{
	cropOffset = 
	{ 
		x, 
		y,  
		w, 
		h 
	};

	SetBox(box.x, box.y, box.w, box.h);
}

void BoxCollider2D::SetBox(int x, int y, int w, int h)
{
	box =
	{
		x - cropOffset.x,
		y - cropOffset.y,
		w - cropOffset.w,
		h - cropOffset.h
	};
}

void BoxCollider2D::DrawBox(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(Renderer::GetRenderer(), r, g, b, a);
	SDL_RenderDrawRect(Renderer::GetRenderer(), &box);
}

void BoxCollider2D::Init()
{
	Collider2D::Init();
}

void BoxCollider2D::Update(float dt)
{
}

void BoxCollider2D::Draw()
{
}

void BoxCollider2D::Clean()
{
}

bool BoxCollider2D::Accept(ColliderVisitor& visitor, Collider2D& other)
{
	return visitor.Visit(*this, other);
}