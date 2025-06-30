#pragma once
#include <chrono>

 
class FrameTimer
{
private:
	static float deltaTime;

	static int currentFPS;


	std::chrono::system_clock::time_point start;

	std::chrono::system_clock::time_point end;

	std::chrono::duration<float> workTime;

	int countedFrames;

	float counterDelay;

	float frameTime;

	bool isFpsLocked;


public:
	FrameTimer();


	void CalculateFPS();

	void StartClock();

	void EndClock();

	void DelayByFrameTime();

	void SetTargetFramerate(unsigned int fps);

	void UnlockFramerate();


	static float DeltaTime();

	static int Framerate();
};
