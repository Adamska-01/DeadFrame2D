#pragma once
#include <box2d/box2d.h>


// Debug Color Legend:
// 
// |		**Element**		|         **Color**			|						**Notes**							|
// | ---------------------- | ------------------------- | --------------------------------------------------------- |
// | Static bodies			| Green						| These don't move (e.g., ground, walls).					|
// | Kinematic bodies		| Blue						| Move manually but not affected by forces.					|
// | Dynamic bodies			| White						| Move with physics forces.									|
// | Sleeping bodies		| Light Gray				| Dynamic but currently inactive (sleeping).				|
// | Sensors (triggers)		| Yellow outline (no fill)	| Detects overlap but doesn’t cause collision.				|
// | Joints					| Pink / Red lines			| Connects two bodies (like springs, hinges, etc).			|
// | AABBs (optional)		| Usually purple/magenta	| Axis-aligned bounding boxes (if enabled).					|
// | Contact points			| Small colored dots		| Often red or orange, shown when contact debug is enabled.	|


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