#pragma once


class ISubSystem
{
	friend class SubSystems;


protected:
	virtual void Update(float deltaTime) = 0;

	virtual void BeginFrame() = 0;

	virtual void EndUpdate() = 0;

	virtual void EndDraw() = 0;
};