#pragma once


class GameObject;


class GameComponent
{
public:
	GameObject* OwningObject;

	bool enableDraw;


	GameComponent();

	GameComponent(GameComponent&& other) = default;

	virtual ~GameComponent() = default;


	virtual void Init() = 0;

	virtual void Update(float dt) = 0;

	virtual void Draw() = 0;


protected:
	virtual void Clean() = 0;
};