#pragma once
#include <Components/GameComponent.h>
#include <memory>
#include <TileEditors/Tiled/Models/TiledLayer.h>


struct TiledMap;
struct TiledObjectGroup;


class GameMapParser : public GameComponent
{
private:
	std::shared_ptr<TiledMap> fullTileMap;


public:
	GameMapParser(std::string_view mapSource);

	virtual ~GameMapParser() = default;


	virtual void Init() override;

	virtual void Update(float dt) override;

	virtual void Draw() override;


	std::shared_ptr<TiledMap> RetrieveRenderMap();
	
	std::vector<TiledLayer> RetrieveCollisionMap();

	std::optional<TiledObjectGroup> RetrieveObjectGroup(std::string_view groupName);

	int GetTileSize() const;
};