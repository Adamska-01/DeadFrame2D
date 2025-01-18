#pragma once


class IObject
{
public:
	virtual ~IObject() = default;


	virtual void Update(float dt) = 0;

	virtual void Draw() = 0;
};