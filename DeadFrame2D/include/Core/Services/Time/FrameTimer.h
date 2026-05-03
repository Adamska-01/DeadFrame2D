#pragma once
#include "DF2D_API.h"
#include <chrono>
#include <optional>


namespace DF2D::Core
{
	class DF2D_API FrameTimer
	{
	private:
		static float deltaTime;

		static float timeScale;

		static int currentFPS;

		static std::chrono::duration<float> workTime;


		std::chrono::system_clock::time_point start;

		std::chrono::system_clock::time_point end;


		int countedFrames;

		float counterDelay;

		float frameTime;

		bool isFpsLocked;


	public:
		FrameTimer(std::optional<int> targetFramerate);


		void CalculateFPS();

		void StartClock();

		void EndClock();

		void DelayByFrameTime();

		void SetTargetFramerate(unsigned int fps);

		void UnlockFramerate();


		static float DeltaTime();

		static float DeltaTimeUnscaled();

		static int Framerate();

		static void SetTimeScale(float scale);

		static float GetTimeScale();
	};
}