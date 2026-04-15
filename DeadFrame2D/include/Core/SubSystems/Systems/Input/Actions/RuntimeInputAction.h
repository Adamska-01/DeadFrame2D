#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Input/Actions/ActionPhase.h"
#include "DF2D_API.h"
#include "Factories/Products/Input/Processors/Abstractions/IInputProcessor.h"
#include "Models/Input/ActionMap/Bindings/Binding.h"
#include "Models/Input/ActionMap/Types/ValueType.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <memory>
#include <string>
#include <variant>
#include <vector>


namespace DeadFrame2D::Core
{
	class InputActionView;


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

		std::vector<Models::Binding> bindings;

		std::vector<std::unique_ptr<Factories::IInputProcessor>> processors;

		Utilities::MulticastDelegate<const InputActionView&> listeners;


		void ResetFrame();

		void ApplyProcessors();


	public:
		RuntimeInputAction(
			const std::string& name,
			Models::ValueType valueType,
			std::vector<Models::Binding> bindings,
			std::vector<Models::InputProcessor> inputProcessors);

		/// No copy allowed (Just enforcing copy restriction from MulticastDelegate)
		RuntimeInputAction(const RuntimeInputAction&) = delete;
		
		RuntimeInputAction& operator=(const RuntimeInputAction&) = delete;

		RuntimeInputAction(RuntimeInputAction&&) noexcept = default;
		
		RuntimeInputAction& operator=(RuntimeInputAction&&) noexcept = default;
	};
}