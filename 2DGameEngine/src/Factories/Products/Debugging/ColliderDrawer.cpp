#include "Constants/PhysicsConstants.h"
#include "Core/SubSystems/Systems/Renderer.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Factories/Products/Debugging/ColliderDrawer.h"
#include <SDL.h>


namespace DeadFrame2D::Factories
{
	using namespace Shared::Constants;

	using namespace DeadFrame2D::Core;


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
		auto sdlColor = SDL_Color(uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255), uint8_t(color.a * 255));

		for (int32 i = 0; i < vertexCount; ++i)
		{
			b2Vec2 p1 = vertices[i];
			b2Vec2 p2 = vertices[(i + 1) % vertexCount];

			TextureManager::DrawLineWorldSpace(
				Vector2F(p1.x * Physics::METER_TO_PIXEL, p1.y * Physics::METER_TO_PIXEL),
				Vector2F(p2.x * Physics::METER_TO_PIXEL, p2.y * Physics::METER_TO_PIXEL),
				sdlColor);
		}
	}

	void ColliderDrawer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		DrawPolygon(vertices, vertexCount, color);
	}

	void ColliderDrawer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
	{
		auto pixelRadius = radius * Physics::METER_TO_PIXEL;
		auto pixelCenter = Vector2F(
			center.x * Physics::METER_TO_PIXEL, 
			center.y * Physics::METER_TO_PIXEL);

		TextureManager::DrawCircleWorldSpace(
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
		auto sdlColor = SDL_Color(uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255), uint8_t(color.a * 255));

		TextureManager::DrawLineWorldSpace(
			Vector2F(p1.x * Physics::METER_TO_PIXEL, p1.y * Physics::METER_TO_PIXEL),
			Vector2F(p2.x * Physics::METER_TO_PIXEL, p2.y * Physics::METER_TO_PIXEL),
			sdlColor);
	}

	void ColliderDrawer::DrawTransform(const b2Transform& xf)
	{
		// Optional: Draw local axes (useful for debugging transforms)
	}

	void ColliderDrawer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
	{
		// TODO: Add this
	}
}