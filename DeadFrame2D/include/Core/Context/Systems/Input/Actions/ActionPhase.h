#pragma once


namespace DeadFrame2D::Core
{
	enum class ActionPhase
	{
		WAITING,

		STARTED,

		PERFORMED,

		CANCELED
	};
}