#pragma once


namespace DF2D::Core
{
	class AudioManager;

	class CoroutineScheduler;

	class TextureManager;

	class Input;

	class PhysicsEngine2D;

	class Renderer;

	class UIManager;

	class Window;
}


namespace DF2D::Data
{
	struct CoreContext
	{
		Core::AudioManager* audioManager;

		Core::CoroutineScheduler* coroutineScheduler;

		Core::TextureManager* textureManager;

		Core::Input* input;

		Core::PhysicsEngine2D* physicsEngine;

		Core::Renderer* renderer;

		Core::UIManager* uiManager;

		Core::Window* window;
	};
}