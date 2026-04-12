#pragma once
#include "Core/SubSystems/Systems/Input/Actions/RuntimeInputAction.h"
#include "DF2D_API.h"
#include <Models/Input/ActionMap/InputActionMap.h>


namespace DeadFrame2D::Core
{
	class DF2D_API RuntimeActionMap
	{
	private:
		std::string name;

		std::vector<RuntimeInputAction> actions;

		bool enabled;


	public:
		RuntimeActionMap(Shared::Models::InputActionMap actionMap);

		/// No copy allowed (Just enforcing copy restriction from RuntimeInputAction)
		RuntimeActionMap(const RuntimeActionMap&) = delete;

		RuntimeActionMap& operator=(const RuntimeActionMap&) = delete;


		void Enable();

		void Disable();

		bool IsEnabled() const;

		const std::string& Name() const;

		std::vector<RuntimeInputAction>& GetActions();
	};
}