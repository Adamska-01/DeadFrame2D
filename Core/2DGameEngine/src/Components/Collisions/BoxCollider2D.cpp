#include "Components/Collisions/BoxCollider2D.h"
#include "Components/Transform.h"
#include "Constants/CommonColors.h"
#include "Data/Collision/CollisionInfo.h"
#include "Factories/Concretions/Debugging/DebugBoxColliderDrawerFactory.h"
#include "Tools/Collisions/ICollisionVisitor.h"
#include "Tools/FrameTimer.h"
#include "Tools/Helpers/EventHelpers.h"


BoxCollider2D::BoxCollider2D(SDL_Rect box, SDL_Rect cropOffset)
	: previousBox(box), box(box), cropOffset(cropOffset)
{
	debugCollisionDrawer = std::unique_ptr<IDebugColliderDrawer<BoxCollider2D>>(std::move(DebugBoxColliderDrawerFactory().CreateProduct(this)));
}

BoxCollider2D::~BoxCollider2D()
{
	OnCollision -= EventHelpers::BindFunction(this, &BoxCollider2D::CollisionHandler);
}


void BoxCollider2D::CollisionHandler(const CollisionInfo& collisionInfo)
{
	// Calculate sliding vector
	auto velocity = transform->position - startFramePosition;

	// Reset to previous position
	transform->position = startFramePosition;

	auto dotProduct = velocity.Dot(collisionInfo.normal);

	// Project velocity onto the collision normal and subtract it to get sliding velocity
	auto slideVelocity = Vector2F
	{
		velocity.x - dotProduct * collisionInfo.normal.x,
		velocity.y - dotProduct * collisionInfo.normal.y
	};

	std::cout << slideVelocity;

	// Apply sliding velocity
	transform->position.x += slideVelocity.x * FrameTimer::DeltaTime() * 200.0f;
	transform->position.y += slideVelocity.y * FrameTimer::DeltaTime() * 200.0f;
}

void BoxCollider2D::UpdatePreviousBox(float x, float y)
{
	SetBox(previousBox, x, y, box.w, box.h);
}

const SDL_Rect& BoxCollider2D::GetCollisionBox() const
{
	return box;
}

const SDL_Rect& BoxCollider2D::GetPreviousBox() const
{
	return previousBox;
}

void BoxCollider2D::SetBox(SDL_Rect& collisionBox, int x, int y, int w, int h)
{
	collisionBox.x = x - cropOffset.x;
	collisionBox.y = y - cropOffset.y;
	collisionBox.w = static_cast<int>(std::round(w - cropOffset.w));
	collisionBox.h = static_cast<int>(std::round(h - cropOffset.h));
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
	SetBox(box, x, y, w, h);
}

void BoxCollider2D::Init()
{
	Collider2D::Init();

	OnCollision += EventHelpers::BindFunction(this, &BoxCollider2D::CollisionHandler);
}

void BoxCollider2D::Draw()
{
	Collider2D::Draw();
	
	SetBox(transform->position.x - (box.w / 2), transform->position.y - (box.h / 2), box.w, box.h);
	
	debugCollisionDrawer->DrawCollider(CommonColors::WHITE);
}

bool BoxCollider2D::Accept(ICollisionVisitor& visitor, Collider2D* other)
{
	SetBox(transform->position.x - (box.w / 2), transform->position.y - (box.h / 2), box.w, box.h);
	UpdatePreviousBox(startFramePosition.x - (box.w / 2), startFramePosition.y - (box.h / 2));

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