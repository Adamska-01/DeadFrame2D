#pragma once
#include "Core/Context/Systems/Coroutines/Task.h"
#include "DF2D_API.h"


namespace DeadFrame2D::Core
{
	class DF2D_API ICoroutineAwaitable
	{
	protected:
		Task* ownerTask = nullptr;


	public:
		virtual ~ICoroutineAwaitable() = default;


		inline void SetOwner(Task* owner);


		virtual bool await_ready() const noexcept = 0;

		virtual void await_suspend(std::coroutine_handle<> h) = 0;

		virtual void await_resume() const noexcept = 0;


		virtual bool Tick(float deltaTime) = 0;
	};


	inline void ICoroutineAwaitable::SetOwner(Task* owner)
	{
		ownerTask = owner;
	}
}