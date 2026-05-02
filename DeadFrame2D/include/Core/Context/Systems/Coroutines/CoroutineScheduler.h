#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "DF2D_API.h"
#include <vector>


namespace DF2D::Core
{
	class ICoroutineAwaitable;
	struct Task;


	class DF2D_API CoroutineScheduler : public ICoreSystem
	{
		friend class SystemInitializer;


	private:
		static CoroutineScheduler* instance;


		std::vector<Task*> tasks;


		CoroutineScheduler();

		~CoroutineScheduler() override;

		CoroutineScheduler(const CoroutineScheduler&) = delete;

		CoroutineScheduler& operator=(const CoroutineScheduler&) = delete;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		static Task& StartCoroutine(Task&& task);

		static void Reset();
	};
}