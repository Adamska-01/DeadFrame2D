#pragma once
#include "Components/Collisions/Collider2D.h"
#include <SDL_rect.h>


struct CollisionInfo;


class BoxCollider2D : public Collider2D
{
private:
	SDL_Rect box;

	SDL_Rect cropOffset;


	void CollisionHandler(const CollisionInfo& collisionInfo);


public:
	BoxCollider2D(SDL_Rect box, SDL_Rect cropOffset);

	virtual ~BoxCollider2D() override;


	const SDL_Rect& GetCollisionBox() const;

	void SetBuffer(int x, int y, int w, int h);

	void SetBox(int x, int y, int w, int h);

	// TODO: Make a damn factory for this, and draw the bow only when running on debug
	void DrawBox(Uint8 r, Uint8 g, Uint8 b, Uint8 a);


	virtual void Init() override;

	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;


	virtual bool Accept(ICollisionVisitor& visitor, Collider2D* other) override;

	virtual bool AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor) override;

	virtual bool AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor) override;

	virtual bool AcceptDispatch(TiledMapCompatibleCollider2D* other, ICollisionVisitor& visitor) override;
};