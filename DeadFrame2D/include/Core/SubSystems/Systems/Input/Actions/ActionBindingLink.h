#pragma once


namespace Shared::Models
{
	struct Binding;
}


namespace DeadFrame2D::Core
{
	class RuntimeInputAction;


	struct ActionBindingLink
	{
		RuntimeInputAction* action;
		
		Shared::Models::Binding* binding;
	};
}