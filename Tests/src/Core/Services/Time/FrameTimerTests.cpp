#include <doctest.h>
#include "Core/Services/Time/Abstractions/IFrameCycle.h"
#include "Core/Services/Time/FrameTimer.h"
#include "Mocks/Services/Time/MockClock.h"
#include <stdexcept>


using namespace DF2D::Core;


static std::unique_ptr<FrameTimer> MakeTimer(std::optional<int> targetFramerate, MockClock*& outClock)
{
	auto clock = std::make_unique<MockClock>();
	outClock = clock.get();

	return std::make_unique<FrameTimer>(targetFramerate, std::move(clock));
}

static void RunFrame(FrameTimer& timer, MockClock& clock, float workSeconds)
{
	auto* frameCycle = static_cast<IFrameCycle*>(&timer);

	frameCycle->BeginFrame();

	clock.Advance(workSeconds);

	frameCycle->EndFrame();
}


TEST_SUITE_BEGIN("FrameTimer");


TEST_CASE("A null clock is rejected")
{
	CHECK_THROWS_AS(FrameTimer(60, nullptr), std::invalid_argument);
}


TEST_CASE("A target framerate locks the timer")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(60, clock);

	CHECK(timer->IsFramerateLocked());
}


TEST_CASE("No target framerate leaves the timer unlocked")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	CHECK_FALSE(timer->IsFramerateLocked());
}


TEST_CASE("A non-positive target framerate leaves the timer unlocked")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(0, clock);

	CHECK_FALSE(timer->IsFramerateLocked());
}


TEST_CASE("The frame delta is the time elapsed between BeginFrame and EndFrame")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	RunFrame(*timer, *clock, 0.25f);

	CHECK(timer->DeltaTimeUnscaled() == doctest::Approx(0.25f));
	CHECK(timer->DeltaTime() == doctest::Approx(0.25f));
}


TEST_CASE("The time scale is applied exactly once to the frame delta")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	timer->SetTimeScale(0.5f);

	RunFrame(*timer, *clock, 0.25f);

	CHECK(timer->DeltaTime() == doctest::Approx(0.125f));
}


TEST_CASE("The unscaled frame delta ignores the time scale")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	timer->SetTimeScale(0.0f);

	RunFrame(*timer, *clock, 0.25f);

	CHECK(timer->DeltaTime() == doctest::Approx(0.0f));
	CHECK(timer->DeltaTimeUnscaled() == doctest::Approx(0.25f));
}


TEST_CASE("The time scale defaults to one and clamps negatives to zero")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	CHECK(timer->GetTimeScale() == doctest::Approx(1.0f));

	timer->SetTimeScale(2.0f);
	CHECK(timer->GetTimeScale() == doctest::Approx(2.0f));

	timer->SetTimeScale(-3.0f);
	CHECK(timer->GetTimeScale() == doctest::Approx(0.0f));
}


TEST_CASE("A locked timer sleeps for the unused part of the frame budget")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(4, clock); // 0.25s per frame

	RunFrame(*timer, *clock, 0.1f);

	REQUIRE(clock->sleepCount == 1);
	CHECK(std::chrono::duration_cast<std::chrono::duration<float>>(clock->sleeps[0]).count() == doctest::Approx(0.15f));
}


TEST_CASE("Slept time is part of the reported frame delta")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(4, clock);

	RunFrame(*timer, *clock, 0.1f);

	CHECK(timer->DeltaTimeUnscaled() == doctest::Approx(0.25f));
}


TEST_CASE("No sleep happens when the frame overruns its budget")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(4, clock);

	RunFrame(*timer, *clock, 0.4f);

	CHECK(clock->sleepCount == 0);
	CHECK(timer->DeltaTimeUnscaled() == doctest::Approx(0.4f));
}


TEST_CASE("An unlocked timer never sleeps")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	RunFrame(*timer, *clock, 0.001f);

	CHECK(clock->sleepCount == 0);
}


TEST_CASE("UnlockFramerate stops the timer from sleeping")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(4, clock);

	timer->UnlockFramerate();

	RunFrame(*timer, *clock, 0.001f);

	CHECK_FALSE(timer->IsFramerateLocked());
	CHECK(clock->sleepCount == 0);
}


TEST_CASE("SetTargetFramerate locks the timer and sets the frame budget")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	timer->SetTargetFramerate(2); // 0.5s per frame

	RunFrame(*timer, *clock, 0.1f);

	CHECK(timer->IsFramerateLocked());
	REQUIRE(clock->sleepCount == 1);
	CHECK(std::chrono::duration_cast<std::chrono::duration<float>>(clock->sleeps[0]).count() == doctest::Approx(0.4f));
}


TEST_CASE("A target framerate of zero is ignored")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(4, clock);

	timer->SetTargetFramerate(0);

	RunFrame(*timer, *clock, 0.1f);

	CHECK(timer->IsFramerateLocked());
	REQUIRE(clock->sleepCount == 1);
	CHECK(std::chrono::duration_cast<std::chrono::duration<float>>(clock->sleeps[0]).count() == doctest::Approx(0.15f));
}


TEST_CASE("The framerate is a usable estimate from the very first frame")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	RunFrame(*timer, *clock, 0.25f);

	CHECK(timer->Framerate() == doctest::Approx(4.0f));
}


TEST_CASE("The framerate reports the frames counted over the last second")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	for (auto i = 0; i < 4; i++)
	{
		RunFrame(*timer, *clock, 0.25f);
	}

	CHECK(timer->Framerate() == doctest::Approx(4.0f));
}


TEST_CASE("The framerate stays accurate when the sample window overshoots one second")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	// 3 normal frames, then one long hitch frame pushes the window well past 1s
	for (auto i = 0; i < 3; i++)
	{
		RunFrame(*timer, *clock, 0.1f);
	}

	RunFrame(*timer, *clock, 5.0f);

	// 4 frames over 5.3s of real time, not the raw frame count (4)
	CHECK(timer->Framerate() == doctest::Approx(4.0f / 5.3f));
}


TEST_CASE("Slept time counts towards the framerate sample window")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(4, clock); // 0.25s per frame, entirely spent sleeping

	for (auto i = 0; i < 4; i++)
	{
		RunFrame(*timer, *clock, 0.0f);
	}

	CHECK(clock->sleepCount == 4);
	CHECK(timer->Framerate() == doctest::Approx(4.0f));
}


TEST_CASE("The frame counter restarts after each sample window")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(std::nullopt, clock);

	for (auto i = 0; i < 4; i++)
	{
		RunFrame(*timer, *clock, 0.25f);
	}

	REQUIRE(timer->Framerate() == doctest::Approx(4.0f));

	for (auto i = 0; i < 2; i++)
	{
		RunFrame(*timer, *clock, 0.5f);
	}

	CHECK(timer->Framerate() == doctest::Approx(2.0f));
}


TEST_CASE("Oversleeping past the requested duration is reflected in the frame delta and framerate")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(4, clock); // 0.25s per frame

	clock->sleepOvershoot = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(0.05f));

	RunFrame(*timer, *clock, 0.1f); // requests a 0.15s sleep, actually sleeps 0.2s

	CHECK(timer->DeltaTimeUnscaled() == doctest::Approx(0.3f));
	CHECK(timer->Framerate() == doctest::Approx(1.0f / 0.3f));
}


TEST_CASE("The sleep margin self-calibrates from observed oversleep instead of using a fixed guess")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(4, clock); // 0.25s per frame

	clock->sleepOvershoot = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(0.05f));

	RunFrame(*timer, *clock, 0.1f); // no data yet: requests the full 0.15s remaining

	REQUIRE(clock->sleepCount == 1);
	CHECK(std::chrono::duration_cast<std::chrono::duration<float>>(clock->sleeps[0]).count() == doctest::Approx(0.15f));

	RunFrame(*timer, *clock, 0.1f); // learned ~0.05s of overshoot: requests correspondingly less

	REQUIRE(clock->sleepCount == 2);
	CHECK(std::chrono::duration_cast<std::chrono::duration<float>>(clock->sleeps[1]).count() == doctest::Approx(0.1f));

	// still lands on the true target despite requesting a shrinking sleep each frame
	CHECK(timer->DeltaTimeUnscaled() == doctest::Approx(0.25f));
}


TEST_CASE("Spinning covers the calibrated margin precisely once oversleep stops happening")
{
	MockClock* clock = nullptr;
	auto timer = MakeTimer(4, clock); // 0.25s per frame

	clock->sleepOvershoot = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(0.05f));

	RunFrame(*timer, *clock, 0.1f); // calibrates the estimate to ~0.05s

	clock->sleepOvershoot = std::chrono::nanoseconds::zero(); // no further oversleep from here

	RunFrame(*timer, *clock, 0.1f); // requests remaining - estimate, spins the rest to hit target

	CHECK(timer->DeltaTimeUnscaled() == doctest::Approx(0.25f));
}


TEST_SUITE_END();