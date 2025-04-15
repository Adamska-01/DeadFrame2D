#pragma once


class GameObject;


class GameComponent
{
	friend class ComponentBucket;


protected:
	bool isActive;

	GameObject* OwningObject;


public:
	GameComponent();

	virtual ~GameComponent() = default;

	GameComponent(GameComponent&& other) = default;


	virtual void Init() = 0;

	virtual void Update(float deltaTime) = 0;

	virtual void Draw() = 0;


	GameObject* GetGameObject() const;

	bool IsActive() const;

	void SetActive(bool value);
};