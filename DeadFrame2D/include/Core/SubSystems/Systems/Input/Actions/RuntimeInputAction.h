#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Input/Actions/ActionPhase.h"
#include "DF2D_API.h"
#include "Models/Input/ActionMap/Bindings/Binding.h"
#include <Models/Input/ActionMap/Types/ValueType.h>
#include <string>
#include <variant>
#include <vector>


namespace DeadFrame2D::Core
{
	class DF2D_API RuntimeInputAction
	{
		friend class InputActionResolver;


	private:
		std::string name;

		std::variant<bool, float, Vector2F> value;

		std::variant<bool, float, Vector2F> previousValue;

		ActionPhase phase;


		std::vector<Shared::Models::Binding> bindings;


		void ResetFrame();


	public:
		RuntimeInputAction(const std::string& name, Shared::Models::ValueType valueType, std::vector<Shared::Models::Binding> bindings);



		template<typename T>
		T ReadValue();

		const std::vector<Shared::Models::Binding>& GetBindings() const;

		bool IsWaiting() const;

		bool IsStarted() const;

		bool IsPerformed() const;

		bool IsCancelled() const;
	};
}


namespace DeadFrame2D::Core
{
	template<typename T>
	inline T RuntimeInputAction::ReadValue()
	{
		if (!std::holds_alternative<T>(value))
			return T{};

		return std::get<T>(value);
	}
}