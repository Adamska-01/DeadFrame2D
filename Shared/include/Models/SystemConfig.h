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
			return SystemConfig
			{
				.window = Shared::Tools::DeserializeFromFile<WindowConfig>(Shared::Constants::Paths::Files::WINDOW_CONFIGURATION),
				.rendering = Shared::Tools::DeserializeFromFile<RendererConfig>(Shared::Constants::Paths::Files::RENDER_CONFIGURATION),
				.audio = Shared::Tools::DeserializeFromFile<AudioConfig>(Shared::Constants::Paths::Files::AUDIO_CONFIGURATION),
				.physics = Shared::Tools::DeserializeFromFile<PhysicsConfig>(Shared::Constants::Paths::Files::PHYSICS_CONFIGURATION)
			};
		}
	};
}