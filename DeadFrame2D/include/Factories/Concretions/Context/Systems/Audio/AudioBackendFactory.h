#pragma once
#include "Core/Context/Systems/Audio/Abstractions/IAudioBackend.h"
#include "Factories/Abstractions/IFactoryWithInitialiser.h"
#include "Models/Audio/AudioConfig.h"
#include <memory>


namespace DF2D::Factories
{
	class AudioBackendFactory : public IFactoryWithInitialiser<std::unique_ptr<Core::IAudioBackend>, Models::AudioConfig>
	{
	public:
		std::unique_ptr<Core::IAudioBackend> CreateProduct(Models::AudioConfig config) override;
	};
}