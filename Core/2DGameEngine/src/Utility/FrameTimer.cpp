#include <thread>
#include <Tools/FrameTimer.h>


using namespace std::chrono;


float FrameTimer::deltaTime = 0;

int FrameTimer::currentFPS = 0;


FrameTimer::FrameTimer()
	: countedFrames(0),
	counterDelay(0),
	isFpsLocked(true)
{
	start = system_clock::now();
	end = system_clock::now();

	SetTargetFramerate(60);
}

void FrameTimer::CalculateFPS()
{
	countedFrames++;
	counterDelay += workTime.count();

	if (counterDelay >= 1000.0f)
	{
		deltaTime = (1.0f / countedFrames);
		currentFPS = countedFrames - 1;

		counterDelay = 0;
		countedFrames = 0;
	}
}

void FrameTimer::StartClock()
{
	start = system_clock::now();
}

void FrameTimer::EndClock()
{
	end = system_clock::now();
	workTime = end - start;
}

void FrameTimer::DelayByFrameTime()
{
	CalculateFPS();

	if (!isFpsLocked || workTime.count() >= frameTime)
		return;

	// Lock framerate to target
	duration<float, std::milli> delta_ms(frameTime - workTime.count());

	auto delta_ms_duration = duration_cast<std::chrono::milliseconds>(delta_ms);

	counterDelay += delta_ms_duration.count(); // For FPS calculation

	std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
}

void FrameTimer::SetTargetFramerate(unsigned int fps)
{
	frameTime = 1000.0f / fps;

	isFpsLocked = true;
}

void FrameTimer::UnlockFramerate()
{
	isFpsLocked = false;
}

float FrameTimer::DeltaTime()
{
	return deltaTime;
}

int FrameTimer::Framerate()
{
	return currentFPS;
}