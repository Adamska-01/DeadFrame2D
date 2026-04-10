#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Input/Actions/ActionPhase.h"
#include "DF2D_API.h"
#include "Factories/Products/Input/Processors/Abstractions/IInputProcessor.h"
#include "Models/Input/ActionMap/Bindings/Binding.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <memory>
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

		std::vector<std::unique_ptr<DeadFrame2D::Factories::IInputProcessor>> processors;

		// TODO: Return a more light-weight class/struct
		DeadFrame2D::Utilities::MulticastDelegate<const RuntimeInputAction&> listeners;


		void ResetFrame();

		void ApplyProcessors();


	public:
		RuntimeInputAction(
			const std::string& name,
			Shared::Models::ValueType valueType,
			std::vector<Shared::Models::Binding> bindings,
			std::vector<Shared::Models::InputProcessor> inputProcessors);

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


	template<typename T>
	inline T RuntimeInputAction::ReadValue() const
	{
		if (!std::holds_alternative<T>(value))
			return T{};

		return std::get<T>(value);
	}
}