#pragma once
#include "GameObjectObserver.h"
#include <memory>


class GameObject;


class GameComponent : public GameObjectObserver
{
	friend class ComponentBucket;


protected:
	mutable bool isDirty;

	bool isActive;

	std::weak_ptr<GameObject> OwningObject;


public:
	GameComponent();

	GameComponent(GameComponent&& other) = default;

	virtual ~GameComponent() override = default;


	virtual void Init() = 0;

	virtual void Start() = 0;

	virtual void Update(float deltaTime) = 0;

	virtual void Draw() = 0;


	virtual void MarkDirty();


	std::weak_ptr<GameObject> GetGameObject() const;

	bool IsActive() const;

	void SetActive(bool value);
};