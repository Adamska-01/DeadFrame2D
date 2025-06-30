#pragma once


class IObject
{
public:
	virtual ~IObject() = default;


	virtual void Init() = 0;

	virtual void Start() = 0;

	virtual void Update(float deltaTime) = 0;

	virtual void LateUpdate(float deltaTime) = 0;

	virtual void Draw() = 0;
};