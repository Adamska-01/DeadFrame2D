#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Input/Actions/ActionPhase.h"
#include "DF2D_API.h"
#include "Models/Input/ActionMap/Bindings/Binding.h"
#include "Utilities/Delegates/MulticastDelegate.h"
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

		std::variant<bool, float, Vector2F> pendingValue;

		bool isValuePending;

		ActionPhase phase;

		std::vector<Shared::Models::Binding> bindings;

		DeadFrame2D::Utilities::MulticastDelegate<const RuntimeInputAction&> listeners;


		void ResetFrame();


	public:
		RuntimeInputAction(const std::string& name, Shared::Models::ValueType valueType, std::vector<Shared::Models::Binding> bindings);

		RuntimeInputAction(const RuntimeInputAction& other);

		RuntimeInputAction(RuntimeInputAction&& other) noexcept;

		RuntimeInputAction& operator=(const RuntimeInputAction& other);

		RuntimeInputAction& operator=(RuntimeInputAction&& other) noexcept;


		template<typename T>
		T ReadValue() const;

		bool IsWaiting() const;

		bool IsStarted() const;

		bool IsPerformed() const;

		bool IsCancelled() const;
	};
}


namespace DeadFrame2D::Core
{
	template<typename T>
	inline T RuntimeInputAction::ReadValue() const
	{
		if (!std::holds_alternative<T>(value))
			return T{};

		return std::get<T>(value);
	}
}