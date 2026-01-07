#pragma once
#include "Data/Rendering/Pipeline/RenderTask.h"
#include "Engine/Components/GameComponent.h"


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
		renderTask.renderPhase = DeadFrame2D::Data::RenderPhase::WORLD;
		renderTask.sortOrder = -1;
	}

	template<typename TMap>
	inline const TMap& TileMapRenderer<TMap>::GetTileMap() const
	{
		return tileMap;
	}
}