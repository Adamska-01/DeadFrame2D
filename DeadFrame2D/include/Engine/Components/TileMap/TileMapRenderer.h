#pragma once
#include "Engine/Components/GameComponent.h"


namespace DeadFrame2D::Engine
{
	template <typename TMap>
	class TileMapRenderer : public GameComponent
	{
		TYPE_INFO(TileMapRenderer<TMap>, GameComponent);


	protected:
		TMap tileMap;


	protected:
		TileMapRenderer() = default;


	public:
		virtual ~TileMapRenderer() = default;


		const TMap& GetTileMap() const;
	};


	template<typename TMap>
	inline const TMap& TileMapRenderer<TMap>::GetTileMap() const
	{
		return tileMap;
	}
}