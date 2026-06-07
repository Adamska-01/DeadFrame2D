#pragma once
#include "Constants/CommonColors.h"
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/Rendering/RenderFlip.h"
#include <optional>


namespace DF2D::Data
{
	struct SpriteRenderData
	{
		Data::TextureID texture = 0;

		std::optional<Core::RectI> srcRect = std::nullopt;

		std::optional<Core::RectF> destRect = std::nullopt;

		Data::RenderFlip flip = Data::RenderFlip::NONE;

		float rotation = 0.0f;

		std::optional<Core::Vector2F> rotationOrigin = std::nullopt;

		Core::Color colorMod = Constants::CommonColors::WHITE;
	};
}