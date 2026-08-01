#pragma once
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsDebugDraw.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/CircleBatchRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/LineBatchRenderData.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/PointBatchRenderData.h"
#include "DF2D_API.h"


// Debug Color Legend:
//
// | ---------------------- | ------------------------- | --------------------------------------------------------- |
// |	  **Element**		|		  **Color**			|						**Notes**							|
// | ---------------------- | ------------------------- | --------------------------------------------------------- |
// | Static bodies			| Green						| These don't move (e.g., ground, walls).					|
// | Kinematic bodies		| Blue						| Move manually but not affected by forces.					|
// | Dynamic bodies			| White						| Move with physics forces.									|
// | Sleeping bodies		| Light Gray				| Dynamic but currently inactive (sleeping).				|
// | Sensors (triggers)		| Yellow outline (no fill)	| Detects overlap but doesn't cause collision.				|
// | Joints					| Pink / Red lines			| Connects two bodies (like springs, hinges, etc).			|
// | AABBs (optional)		| Usually purple/magenta	| Axis-aligned bounding boxes (if enabled).					|
// | Contact points			| Small colored dots		| Often red or orange, shown when contact debug is enabled.	|
// | ---------------------- | ------------------------- | --------------------------------------------------------- |


namespace DF2D::Core
{
	class DF2D_API PhysicsDebugDrawer : public IPhysicsDebugDraw
	{
	private:
		Data::RenderTask renderTask;

		Data::LineBatchRenderData lineBatchData;

		Data::PointBatchRenderData pointBatchData;

		Data::CircleBatchRenderData circleBatchData;


	public:
		PhysicsDebugDrawer();

		~PhysicsDebugDrawer() override = default;


		void DrawSegment(const Vector2F& p1, const Vector2F& p2, const Color& color) override;

		void DrawCircle(const Vector2F& center, float radius, const Color& color) override;

		void DrawPoint(const Vector2F& position, float size, const Color& color) override;


		void Flush();
	};
}