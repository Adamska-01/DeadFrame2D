#include "Constants/Rendering/DefaultSortOrders.h"
#include "Core/Context/Systems/Physics/Debugging/PhysicsDebugDrawer.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include <cmath>


namespace DF2D::Core
{
	using namespace DF2D::Constants;
	using namespace DF2D::Data;


	PhysicsDebugDrawer::PhysicsDebugDrawer()
	{
		renderTask.renderPhase = RenderPhase::DEBUG_WORLD;
		renderTask.sortOrder = DefaultSortOrders::DEBUG_RENDERER;
	}

	void PhysicsDebugDrawer::DrawSegment(const Vector2F& p1, const Vector2F& p2, const Color& color)
	{
		auto lineRenderData = LineRenderData
		{
			.p1 = p1,
			.p2 = p2,
			.color = color
		};

		lineBatchData.lineBatch.push_back(lineRenderData);
	}

	void PhysicsDebugDrawer::DrawCircle(const Vector2F& center, float radius, bool filled, const Color& color)
	{
		auto circleRenderData = CircleRenderData
		{
			.center = center,
			.radius = radius,
			.filled = filled,
			.color = color
		};

		circleBatchData.circleBatch.push_back(circleRenderData);
	}

	void PhysicsDebugDrawer::DrawPoint(const Vector2F& position, float size, const Color& color)
	{
		auto halfSize = static_cast<int>(std::round(size * 0.5f));

		for (auto dx = -halfSize; dx <= halfSize; ++dx)
		{
			for (auto dy = -halfSize; dy <= halfSize; ++dy)
			{
				auto pointData = PointRenderData
				{
					.pos = position + Vector2F(float(dx), float(dy)),
					.color = color
				};

				pointBatchData.pointBatch.push_back(pointData);
			}
		}
	}

	void PhysicsDebugDrawer::Flush()
	{
		if (!lineBatchData.lineBatch.empty())
		{
			renderTask.renderData = std::move(lineBatchData);

			RenderSystem::Submit(renderTask);

			lineBatchData.lineBatch.clear();
		}

		if (!pointBatchData.pointBatch.empty())
		{
			renderTask.renderData = std::move(pointBatchData);

			RenderSystem::Submit(renderTask);

			pointBatchData.pointBatch.clear();
		}

		if (!circleBatchData.circleBatch.empty())
		{
			renderTask.renderData = std::move(circleBatchData);

			RenderSystem::Submit(renderTask);

			circleBatchData.circleBatch.clear();
		}
	}
}