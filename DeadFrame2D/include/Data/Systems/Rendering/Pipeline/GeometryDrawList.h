#pragma once
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/Rendering/BlendMode.h"
#include "Data/Systems/Rendering/Vertex2D.h"
#include <cstdint>
#include <optional>
#include <vector>


namespace DF2D::Data
{
	/**
	 * @brief One indexed draw taken out of a GeometryDrawList's shared vertex and index pools.
	 */
	struct GeometryCommand
	{
		/** @brief Texture to sample, or 0 to draw untextured using vertex colors only. */
		TextureID texture = 0;

		uint32_t vertexOffset = 0;

		uint32_t vertexCount = 0;

		uint32_t indexOffset = 0;

		uint32_t indexCount = 0;

		/** @brief Offset added to every vertex position at draw time. */
		Core::Vector2F translation;

		/** @brief Scissor rectangle in render-target space; std::nullopt disables clipping. */
		std::optional<Core::RectI> clipRect = std::nullopt;

		BlendMode blendMode = BlendMode::ALPHA;
	};


	/**
	 * @brief An ordered command buffer of indexed triangle meshes sharing one vertex and index pool.
	 *
	 * Commands are replayed strictly in submission order. Producers rely on painter's-algorithm
	 * ordering and carry stateful scissor rectangles, so a draw list must never be reordered, sorted
	 * or split across several render tasks.
	 */
	struct GeometryDrawList
	{
		std::vector<Vertex2D> vertices;

		std::vector<uint32_t> indices;

		std::vector<GeometryCommand> commands;
	};
}