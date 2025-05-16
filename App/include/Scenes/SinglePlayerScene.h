#pragma once
#include <Management/Scene.h>


class SinglePlayerScene : public Scene
{
public:
	SinglePlayerScene() = default;

	virtual ~SinglePlayerScene() override = default;


	virtual void Enter() override;
};