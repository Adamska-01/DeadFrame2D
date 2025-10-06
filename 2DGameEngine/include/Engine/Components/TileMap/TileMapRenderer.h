#pragma once
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"


namespace DeadFrame2D::Engine
{
	template <typename TMap>
	class DF2D_API TileMapRenderer : public GameComponent
	{
	protected:
		TMap tileMap;


	protected:
		TileMapRenderer() = default;


	public:
		virtual ~TileMapRenderer() = default;


		virtual void Init() = 0;

		virtual void Start() = 0;

		virtual void Update(float dt) = 0;

		virtual void Draw() = 0;


		const TMap& GetTileMap() const;
	};


	template<typename TMap>
	inline const TMap& TileMapRenderer<TMap>::GetTileMap() const
	{
		return tileMap;
	}
}