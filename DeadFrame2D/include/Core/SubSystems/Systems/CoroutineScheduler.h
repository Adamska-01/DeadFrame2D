#pragma once
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include <coroutine>
#include <memory>
#include <vector>


namespace DeadFrame2D::Core
{
	class ICoroutineAwaitable;
	struct Task;


	class DF2D_API CoroutineScheduler : public ISubSystem
	{
		friend class SubSystemManager;


	private:
		static CoroutineScheduler* instance;


		std::vector<Task*> tasks;


		CoroutineScheduler();

		virtual ~CoroutineScheduler() override;

		CoroutineScheduler(const CoroutineScheduler&) = delete;

		CoroutineScheduler& operator=(const CoroutineScheduler&) = delete;


		virtual void Update(float deltaTime) override;

		virtual void BeginFrame() override;

		virtual void EndUpdate() override;

		virtual void EndDraw() override;


	public:
		static Task& StartCoroutine(Task&& task);

		static void Reset();
	};
}