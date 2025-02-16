#include "Components/Collisions/BoxCollider2D.h"
#include "Components/Transform.h"
#include "SubSystems/Renderer.h"
#include "Tools/Collisions/ICollisionVisitor.h"


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
	box.x = x;
	box.y = y;
	box.w = w;
	box.h = h;

	SetBox(box.x, box.y, box.w, box.h);
}

void BoxCollider2D::SetBox(int x, int y, int w, int h)
{
	box.x = x - cropOffset.x;
	box.y = y - cropOffset.y;
	box.w = static_cast<int>(std::round(w - cropOffset.w));
	box.h = static_cast<int>(std::round(h - cropOffset.h));
}

void BoxCollider2D::DrawBox(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Renderer* renderer = Renderer::GetRenderer();

	Uint8 oldR, oldG, oldB, oldA;
	SDL_GetRenderDrawColor(renderer, &oldR, &oldG, &oldB, &oldA);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_RenderDrawRect(renderer, &box);

	SDL_SetRenderDrawColor(renderer, oldR, oldG, oldB, oldA);
}

void BoxCollider2D::Init()
{
	Collider2D::Init();
}

void BoxCollider2D::Update(float dt)
{
	Collider2D::Update(dt);

	SetBox(previousPosition.x - (box.w / 2), previousPosition.y - (box.h / 2), box.w, box.h);
}

void BoxCollider2D::Draw()
{
	DrawBox(255, 255, 255, 255);
}

void BoxCollider2D::Clean()
{
}

bool BoxCollider2D::Accept(ICollisionVisitor& visitor, Collider2D* other)
{
	return other->AcceptDispatch(this, visitor);
}

bool BoxCollider2D::AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor)
{
	return visitor.Visit(this, other);
}

bool BoxCollider2D::AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor)
{
	return visitor.Visit(this, other);
}

bool BoxCollider2D::AcceptDispatch(TiledMapCompatibleCollider2D* other, ICollisionVisitor& visitor)
{
	return visitor.Visit(this, other);
}