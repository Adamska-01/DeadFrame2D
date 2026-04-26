#pragma once
#include "Constants/Rendering/DefaultSortOrders.h"
#include "Data/Rendering/Pipeline/RenderTask.h"
#include "Engine/ECS/Component/GameComponent.h"


namespace DeadFrame2D::Engine
{
	template <typename TMap>
	class TileMapRenderer : public GameComponent
	{
		TYPE_INFO(TileMapRenderer<TMap>, GameComponent);


	protected:
		TMap tileMap;

		DeadFrame2D::Data::RenderTask renderTask;


	protected:
		TileMapRenderer();


	public:
		virtual ~TileMapRenderer() = default;


		const TMap& GetTileMap() const;
	};
}


namespace DeadFrame2D::Engine
{
	template<typename TMap>
	inline TileMapRenderer<TMap>::TileMapRenderer()
	{
		using namespace DeadFrame2D::Constants;
		using namespace DeadFrame2D::Data;


		renderTask.renderPhase = RenderPhase::WORLD;
		renderTask.sortOrder = DefaultSortOrders::TILE_RENDERER;
	}

	template<typename TMap>
	inline const TMap& TileMapRenderer<TMap>::GetTileMap() const
	{
		return tileMap;
	}
}