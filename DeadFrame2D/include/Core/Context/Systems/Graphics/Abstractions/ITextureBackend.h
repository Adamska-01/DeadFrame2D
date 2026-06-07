#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "DF2D_API.h"
#include <string>


namespace DF2D::Core
{
	class DF2D_API ITextureBackend
	{
	public:
		virtual ~ITextureBackend() = default;


		virtual Data::TextureID LoadFromFile(const std::string& filename) = 0;

		virtual void UnloadTexture(Data::TextureID id) = 0;

		virtual Vector2I GetTextureSize(Data::TextureID id) = 0;
	};
}
