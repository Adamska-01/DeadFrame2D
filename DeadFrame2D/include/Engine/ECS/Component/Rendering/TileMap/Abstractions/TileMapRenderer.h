#pragma once
#include "Constants/Rendering/DefaultSortOrders.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"


namespace DF2D::Engine
{
	template <typename TMap>
	class TileMapRenderer : public GameComponent
	{
		TYPE_INFO(TileMapRenderer<TMap>, GameComponent);


	protected:
		TMap tileMap;

		Data::RenderTask renderTask;


	protected:
		TileMapRenderer();


	public:
		virtual ~TileMapRenderer() = default;


		const TMap& GetTileMap() const;
	};
}


namespace DF2D::Engine
{
	template<typename TMap>
	inline TileMapRenderer<TMap>::TileMapRenderer()
	{
		using namespace DF2D::Constants;
		using namespace DF2D::Data;


		renderTask.renderPhase = RenderPhase::WORLD;
		renderTask.sortOrder = DefaultSortOrders::TILE_RENDERER;
	}

	template<typename TMap>
	inline const TMap& TileMapRenderer<TMap>::GetTileMap() const
	{
		return tileMap;
	}
}