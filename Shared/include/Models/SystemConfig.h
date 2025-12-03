#pragma once
#include "Constants/ResourcePaths.h"
#include "Models/Audio/AudioConfig.h"
#include "Models/Rendering/RendererConfig.h"
#include "Models/Window/WindowConfig.h"
#include "Physics/PhysicsConfig.h"
#include "Tools/Serialization/JsonSerializer.h"


namespace Shared::Models
{
	struct SystemConfig
	{
		WindowConfig window;

		RendererConfig rendering;

		AudioConfig audio;

		PhysicsConfig physics;



		static SystemConfig LoadFromFiles()
		{
			using namespace Shared::Constants::Paths;


			return SystemConfig
			{
				.window = Shared::Tools::DeserializeFromFile<WindowConfig>(Files::WINDOW_CONFIGURATION),
				.rendering = Shared::Tools::DeserializeFromFile<RendererConfig>(Files::RENDER_CONFIGURATION),
				.audio = Shared::Tools::DeserializeFromFile<AudioConfig>(Files::AUDIO_CONFIGURATION),
				.physics = Shared::Tools::DeserializeFromFile<PhysicsConfig>(Files::PHYSICS_CONFIGURATION)
			};
		}
	};
}