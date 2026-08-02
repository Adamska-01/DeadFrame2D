#pragma once


namespace DF2D::Core
{
	class EventManager;

	class FrameTimer;

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

		Core::FrameTimer* frameTimer;

		Core::SystemDialogBox* systemDialogBox;

		Engine::SceneManager* sceneManager;
	};
}