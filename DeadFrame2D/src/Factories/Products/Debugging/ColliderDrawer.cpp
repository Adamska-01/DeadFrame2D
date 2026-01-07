#include "Core/SubSystems/Systems/Physics/PhysicsEngine2D.h"
#include "Core/SubSystems/Systems/Rendering/Renderer.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Factories/Products/Debugging/ColliderDrawer.h"


namespace DeadFrame2D::Factories
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;


	ColliderDrawer::ColliderDrawer()
	{
		renderTask.renderPhase = RenderPhase::DEBUG_WORLD;
		// TODO: Make some constants for this stuff
		renderTask.sortOrder = 9999;
		
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
			auto p1 = vertices[i];
			auto p2 = vertices[(i + 1) % vertexCount];

			const auto PIXEL_PER_METER = PhysicsEngine2D::GetPhysicsConfig().pixelPerMeter;

			renderTask.renderData = LineRenderData
			{
				.p1 = Vector2F(p1.x * PIXEL_PER_METER, p1.y * PIXEL_PER_METER),
				.p2 = Vector2F(p2.x * PIXEL_PER_METER, p2.y * PIXEL_PER_METER),
				.color = sdlColor
			};

			RenderSystem::Submit(renderTask);
		}
	}

	void ColliderDrawer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		DrawPolygon(vertices, vertexCount, color);
	}

	void ColliderDrawer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
	{
		const auto PIXEL_PER_METER = PhysicsEngine2D::GetPhysicsConfig().pixelPerMeter;

		auto pixelRadius = radius * PIXEL_PER_METER;
		auto pixelCenter = Vector2F(
			center.x * PIXEL_PER_METER,
			center.y * PIXEL_PER_METER);

		renderTask.renderData = CircleRenderData
		{
			.center = pixelCenter,
			.radius = pixelRadius,
			.filled = false,
			.color = SDL_Color{ Uint8(color.r * 255), Uint8(color.g * 255), Uint8(color.b * 255), Uint8(color.a * 255) }
		};

		RenderSystem::Submit(renderTask);
	}

	void ColliderDrawer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
	{
		DrawCircle(center, radius, color);
	}

	void ColliderDrawer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		const auto PIXEL_PER_METER = PhysicsEngine2D::GetPhysicsConfig().pixelPerMeter;
		
		auto sdlColor = SDL_Color(uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255), uint8_t(color.a * 255));

		renderTask.renderData = LineRenderData
		{
			.p1 = Vector2F(p1.x * PIXEL_PER_METER, p1.y * PIXEL_PER_METER),
			.p2 = Vector2F(p2.x * PIXEL_PER_METER, p2.y * PIXEL_PER_METER),
			.color = sdlColor
		};

		RenderSystem::Submit(renderTask);
	}

	void ColliderDrawer::DrawTransform(const b2Transform& xf)
	{
		DrawPoint(xf.p, 2.0f, b2Color{ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void ColliderDrawer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
	{
		const auto PIXEL_PER_METER = PhysicsEngine2D::GetPhysicsConfig().pixelPerMeter;

		auto center = Vector2F(p.x * PIXEL_PER_METER, p.y * PIXEL_PER_METER);

		auto sdlColor = SDL_Color(uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255), uint8_t(color.a * 255));

		auto halfSize = static_cast<int>(std::round(size * 0.5f));

		for (auto dx = -halfSize; dx <= halfSize; ++dx)
		{
			for (auto dy = -halfSize; dy <= halfSize; ++dy)
			{
				renderTask.renderData = PointRenderData
				{
					.pos = center + Vector2F(float(dx), float(dy)),
					.color = sdlColor
				};

				RenderSystem::Submit(renderTask);
			}
		}
	}
}