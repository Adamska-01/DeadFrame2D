#pragma once
#include <Components/GameComponent.h>
#include <string>
#include <vector>


class GameManager : public GameComponent
{
private:
	std::vector<std::string> levelSources;


public:
	GameManager();

	virtual ~GameManager() override = default;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;
};