#pragma once
#include "EventSystem/DispatchableEvent.h"


class Cannon;


class LoadNewBobbleEvent : public DispatchableEvent
{
public:
	Cannon* interestedCannon;


	LoadNewBobbleEvent(Cannon* interestedCannon);
};