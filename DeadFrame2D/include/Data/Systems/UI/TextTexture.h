#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"


namespace DF2D::Data
{
	struct TextTexture
	{
		TextureID textureID = 0;

		Core::Vector2I size = Core::Vector2I::Zero;
	};
}