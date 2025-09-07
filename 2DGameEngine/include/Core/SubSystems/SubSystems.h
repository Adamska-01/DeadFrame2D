#pragma once
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include <Models/Engine/EngineConfig.h>


// TODO: Rename this to 'SubSystemManager'
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