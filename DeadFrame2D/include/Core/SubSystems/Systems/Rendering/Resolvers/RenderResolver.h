#pragma once
#include "Core/SubSystems/Systems/Rendering/Resolvers/Functors/CircleBatchRenderResolver.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/Functors/CircleRenderResolver.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/Functors/LineBatchRenderResolver.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/Functors/LineRenderResolver.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/Functors/PointBatchRenderResolver.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/Functors/PointRenderResolver.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/Functors/RectRenderResolver.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/Functors/SpriteBatchRenderResolver.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/Functors/SpriteRenderResolver.h"
#include <type_traits>


namespace DeadFrame2D::Core::RenderResolver
{
	template<typename T>
	inline auto& GetRenderResolver()
	{
		using namespace DeadFrame2D::Data;


		static_assert(
			std::is_same_v<T, SpriteRenderData>
			|| std::is_same_v<T, RectRenderData>
			|| std::is_same_v<T, LineRenderData>
			|| std::is_same_v<T, CircleRenderData>
			|| std::is_same_v<T, PointRenderData>
			|| std::is_same_v<T, LineBatchRenderData>
			|| std::is_same_v<T, PointBatchRenderData>
			|| std::is_same_v<T, CircleBatchRenderData>
			|| std::is_same_v<T, SpriteBatchRenderData>,
			"No Render Resolver for this RenderData type");


		if constexpr (std::is_same_v<T, SpriteRenderData>)
		{
			static SpriteRenderResolver spriteInstance;

			return spriteInstance;
		}
		else if constexpr (std::is_same_v<T, RectRenderData>)
		{
			static RectRenderResolver rectInstance;

			return rectInstance;
		}
		else if constexpr (std::is_same_v<T, LineRenderData>)
		{
			static LineRenderResolver lineInstance;

			return lineInstance;
		}
		else if constexpr (std::is_same_v<T, CircleRenderData>)
		{
			static CircleRenderResolver circleInstance;

			return circleInstance;
		}
		else if constexpr (std::is_same_v<T, PointRenderData>)
		{
			static PointRenderResolver pointInstance;

			return pointInstance;
		}
		else if constexpr (std::is_same_v<T, LineBatchRenderData>)
		{
			static LineBatchRenderResolver lineBatchInstance;
			
			return lineBatchInstance;
		}
		else if constexpr (std::is_same_v<T, PointBatchRenderData>)
		{
			static PointBatchRenderResolver pointBatchInstance;

			return pointBatchInstance;
		}
		else if constexpr (std::is_same_v<T, CircleBatchRenderData>)
		{
			static CircleBatchRenderResolver circleBatchInstance;

			return circleBatchInstance;
		}
		else if constexpr (std::is_same_v<T, SpriteBatchRenderData>)
		{
			static SpriteBatchRenderResolver circleBatchInstance;

			return circleBatchInstance;
		}
	};
}