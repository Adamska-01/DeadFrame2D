#pragma once
#include "Core/Context/Systems/Input/Actions/ActionPhase.h"
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include <variant>


namespace DF2D::Core
{
	class DF2D_API InputActionView
	{
	private:
		ActionPhase phase;

		std::variant<bool, float, Vector2F> value;


	public:
		InputActionView(ActionPhase phase, std::variant<bool, float, Vector2F> value);


		template<typename T>
		T ReadValue() const;

		bool IsWaiting() const;

		bool IsStarted() const;

		bool IsPerformed() const;

		bool IsCancelled() const;
	};
}


namespace DF2D::Core
{
	template<typename T>
	inline T InputActionView::ReadValue() const
	{
		if (!std::holds_alternative<T>(value))
			return T{};

		return std::get<T>(value);
	}
}