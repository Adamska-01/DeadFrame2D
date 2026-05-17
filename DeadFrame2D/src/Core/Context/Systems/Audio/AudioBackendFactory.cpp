#include "Factories/Concretions/Context/Systems/Audio/AudioBackendFactory.h"
#include "Factories/Products/Context/Systems/Audio/SDLAudioBackend.h"


namespace DF2D::Factories
{
	using namespace DF2D::Core;
	using namespace DF2D::Models;
	using namespace DF2D::Internal;


	std::unique_ptr<IAudioBackend> AudioBackendFactory::CreateProduct(AudioConfig config)
	{
		return std::make_unique<SDLAudioBackend>(config);
	}
}