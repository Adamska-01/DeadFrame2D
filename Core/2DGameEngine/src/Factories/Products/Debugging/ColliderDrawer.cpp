#include "Constants/PhysicsConstants.h"
#include "Factories/Products/Debugging/ColliderDrawer.h"
#include "SubSystems/Renderer.h"
#include "SubSystems/TextureManager.h"
#include <SDL.h>


ColliderDrawer::ColliderDrawer(SDL_Renderer* renderer)
	: renderer(renderer)
{
	SetFlags(
		b2Draw::e_shapeBit |
		b2Draw::e_jointBit |
		b2Draw::e_aabbBit |
		b2Draw::e_pairBit |
		b2Draw::e_centerOfMassBit);
}

void ColliderDrawer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	auto oldRenderColor = Renderer::GetDisplayColor();

	Renderer::SetDisplayColor(Uint8(color.r * 255), Uint8(color.g * 255), Uint8(color.b * 255), Uint8(color.a * 255));

	for (int32 i = 0; i < vertexCount; ++i)
	{
		b2Vec2 p1 = vertices[i];
		b2Vec2 p2 = vertices[(i + 1) % vertexCount];

		SDL_RenderDrawLine(renderer,
			static_cast<int>(p1.x * PhysicsConstants::METER_TO_PIXEL),
			static_cast<int>(p1.y * PhysicsConstants::METER_TO_PIXEL),
			static_cast<int>(p2.x * PhysicsConstants::METER_TO_PIXEL),
			static_cast<int>(p2.y * PhysicsConstants::METER_TO_PIXEL));
	}

	// Reset Display Color
	Renderer::SetDisplayColor(oldRenderColor.r, oldRenderColor.g, oldRenderColor.b, oldRenderColor.a);
}

void ColliderDrawer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	DrawPolygon(vertices, vertexCount, color);
}

void ColliderDrawer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	auto pixelRadius = radius * PhysicsConstants::METER_TO_PIXEL;
	auto pixelCenter = Vector2F(
		center.x * PhysicsConstants::METER_TO_PIXEL, 
		center.y * PhysicsConstants::METER_TO_PIXEL);

	TextureManager::DrawCircle(
		Circle(pixelCenter, pixelRadius),
		SDL_Color{ Uint8(color.r * 255), Uint8(color.g * 255), Uint8(color.b * 255), Uint8(color.a * 255) },
		false);
}

void ColliderDrawer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	DrawCircle(center, radius, color);
}

void ColliderDrawer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	auto oldRenderColor = Renderer::GetDisplayColor();

	Renderer::SetDisplayColor(Uint8(color.r * 255), Uint8(color.g * 255), Uint8(color.b * 255), Uint8(color.a * 255));

	SDL_RenderDrawLine(renderer,
		static_cast<int>(p1.x * PhysicsConstants::METER_TO_PIXEL),
		static_cast<int>(p1.y * PhysicsConstants::METER_TO_PIXEL),
		static_cast<int>(p2.x * PhysicsConstants::METER_TO_PIXEL),
		static_cast<int>(p2.y * PhysicsConstants::METER_TO_PIXEL));

	// Reset Display Color
	Renderer::SetDisplayColor(oldRenderColor.r, oldRenderColor.g, oldRenderColor.b, oldRenderColor.a);
}

void ColliderDrawer::DrawTransform(const b2Transform& xf)
{
	// Optional: Draw local axes (useful for debugging transforms)
}

void ColliderDrawer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{

}