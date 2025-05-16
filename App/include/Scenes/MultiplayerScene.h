#pragma once
#include <Management/Scene.h>


class MultiplayerScene : public Scene
{
public:
	MultiplayerScene() = default;

	virtual ~MultiplayerScene() override = default;


	virtual void Enter() override;
};