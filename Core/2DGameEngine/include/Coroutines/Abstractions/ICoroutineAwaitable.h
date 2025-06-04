#pragma once


class ICoroutineAwaitable 
{
public:
	virtual ~ICoroutineAwaitable() = default;
 
	virtual bool Tick(float dt) = 0;
};