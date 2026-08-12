#pragma once


namespace DF2D::Core
{
	class EventManager;

	class ITimeProvider;

	class SystemDialogBox;
}


namespace DF2D::Engine
{
	class SceneManager;
}


namespace DF2D::Data
{
	struct ServiceContext
	{
		Core::EventManager* eventManager;

		Core::ITimeProvider* frameTimer;

		Core::SystemDialogBox* systemDialogBox;

		Engine::SceneManager* sceneManager;
	};
}