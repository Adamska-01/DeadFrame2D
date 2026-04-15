#pragma once
#include "Constants/Paths/ResourcePaths.h"
#include "Models/Audio/AudioConfig.h"
#include "Models/Rendering/RendererConfig.h"
#include "Models/Window/WindowConfig.h"
#include "Physics/PhysicsConfig.h"
#include "Utilities/IO/Serialization/JsonSerializer.h"


namespace DeadFrame2D::Models
{
	struct SystemConfig
	{
		WindowConfig window;

		RendererConfig rendering;

		AudioConfig audio;

		PhysicsConfig physics;



		static SystemConfig LoadFromFiles()
		{
			using namespace DeadFrame2D::Constants;
			using namespace DeadFrame2D::Utilities;


			return SystemConfig
			{
				.window = JsonSerializer::DeserializeFromFile<WindowConfig>(Paths::Files::WINDOW_CONFIGURATION),
				.rendering = JsonSerializer::DeserializeFromFile<RendererConfig>(Paths::Files::RENDER_CONFIGURATION),
				.audio = JsonSerializer::DeserializeFromFile<AudioConfig>(Paths::Files::AUDIO_CONFIGURATION),
				.physics = JsonSerializer::DeserializeFromFile<PhysicsConfig>(Paths::Files::PHYSICS_CONFIGURATION)
			};
		}
	};
}