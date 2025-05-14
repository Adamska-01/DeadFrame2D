#pragma once
#include <box2d/box2d.h>


struct SDL_Renderer;


class ColliderDrawer : public b2Draw
{
private:
	SDL_Renderer* renderer;


public:
	ColliderDrawer(SDL_Renderer* renderer);
	
	~ColliderDrawer() override = default;


	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
	
	virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	virtual void DrawTransform(const b2Transform& xf) override;

	virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
};