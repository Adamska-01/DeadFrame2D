#include "Factories/Products/Services/Time/SteadyClock.h"
#include <thread>


namespace DF2D::Internal
{
	using namespace std::chrono;


	nanoseconds SteadyClock::Now() const
	{
		return steady_clock::now().time_since_epoch();
	}

	void SteadyClock::SleepFor(nanoseconds duration)
	{
		if (duration <= nanoseconds::zero())
			return;

		std::this_thread::sleep_for(duration);
	}
}