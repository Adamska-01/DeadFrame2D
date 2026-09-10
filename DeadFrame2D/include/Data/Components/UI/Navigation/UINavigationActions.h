#pragma once
#include <string>


namespace DF2D::Data
{
	// TODO: Create a config file (json) that overrides these
	/**
	 * @brief Which input actions a UINavigator listens to.
	 */
	struct UINavigationActions
	{
		std::string actionMap = "UI";

		/** @brief Vector2 action moving focus between widgets. */
		std::string navigate = "Navigate";

		/** @brief Button action activating whatever holds focus. */
		std::string submit = "Submit";

		/** @brief Button action asking to back out. The engine only reports it; the game decides. */
		std::string cancel = "Cancel";
	};
}