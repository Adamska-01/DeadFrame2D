#pragma once
#include "Core/Services/Time/Abstractions/IClock.h"
#include "DF2D_API.h"


namespace DF2D::Internal
{
	class DF2D_API SteadyClock : public Core::IClock
	{
	public:
		SteadyClock() = default;

		~SteadyClock() override = default;


		std::chrono::nanoseconds Now() const override;

		void SleepFor(std::chrono::nanoseconds duration) override;
	};
}