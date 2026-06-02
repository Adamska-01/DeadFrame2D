#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Services/Time/FrameTimer.h"
#include "DF2D_API.h"
#include <vector>


namespace DF2D::Core
{
	class ICoroutineAwaitable;
	struct Task;


	class DF2D_API CoroutineScheduler : public ICoreSystem
	{
	private:
		std::vector<Task*> tasks;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		CoroutineScheduler() = default;

		~CoroutineScheduler() override;

		CoroutineScheduler(const CoroutineScheduler&) = delete;

		CoroutineScheduler& operator=(const CoroutineScheduler&) = delete;




		Task& StartCoroutine(Task&& task);

		void Reset();
	};
}