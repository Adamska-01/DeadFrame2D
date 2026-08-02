#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Services/Time/Abstractions/ITimeProvider.h"
#include "DF2D_API.h"
#include <vector>


namespace DF2D::Engine
{
	class SceneManager;
}


namespace DF2D::Core
{
	class ICoroutineAwaitable;
	struct Task;


	class DF2D_API CoroutineScheduler : public ICoreSystem
	{
		friend class Engine::SceneManager;


	private:
		const ITimeProvider* timeProvider;

		std::vector<Task*> tasks;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


		void Reset();


	public:
		CoroutineScheduler(const ITimeProvider* timeProvider);

		~CoroutineScheduler() override;

		CoroutineScheduler(const CoroutineScheduler&) = delete;

		CoroutineScheduler& operator=(const CoroutineScheduler&) = delete;


		Task& StartCoroutine(Task&& task);

		size_t GetActiveTaskCount() const;
	};
}