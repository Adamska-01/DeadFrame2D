#pragma once
#include <Components/GameComponent.h>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>


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


	std::shared_ptr<TiledMap> RetrieveRenderMap(std::vector<std::string_view> layerNames);
	
	std::optional<TiledObjectGroup> RetrieveObjectGroup(std::string_view groupName);

	int GetTileSize() const;
};