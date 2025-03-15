#pragma once
#include "Components/Collisions/Collider2D.h"
#include "Factories/Abstractions/Debugging/IDebugColliderDrawer.h"
#include <SDL_rect.h>


struct CollisionInfo;


class BoxCollider2D : public Collider2D
{
private:
	SDL_Rect previousBox;

	SDL_Rect box;

	SDL_Rect cropOffset;

	std::unique_ptr<IDebugColliderDrawer<BoxCollider2D>> debugCollisionDrawer;


	void CollisionHandler(const CollisionInfo& collisionInfo);

	void UpdatePreviousBox(float x, float y);

	void SetBox(SDL_Rect& collisionBox, int x, int y, int w, int h);


public:
	BoxCollider2D(SDL_Rect box, SDL_Rect cropOffset);

	virtual ~BoxCollider2D() override;


	const SDL_Rect& GetCollisionBox() const;

	const SDL_Rect& GetPreviousBox() const;

	void SetBuffer(int x, int y, int w, int h);

	void SetBox(int x, int y, int w, int h);


	virtual void Init() override;

	virtual void Draw() override;


	virtual bool Accept(ICollisionVisitor& visitor, Collider2D* other) override;

	virtual bool AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor) override;

	virtual bool AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor) override;

	virtual bool AcceptDispatch(TiledMapCompatibleCollider2D* other, ICollisionVisitor& visitor) override;
};