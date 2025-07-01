#pragma once
#include <Models/Engine/EngineConfig.h>
#include "SubSystems/Abstractions/ISubSystem.h"


class SubSystems
{
private:
	std::array<ISubSystem*, 8> subSystems;


public:
	SubSystems();
	
	~SubSystems();


	void InitializeSubSystems(EngineConfig config);


	void Update(float deltaTime);

	void BeginFrame();

	void EndUpdate();

	void EndDraw();
};