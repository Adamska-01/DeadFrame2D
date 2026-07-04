#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"


struct MockRenderBackend : DF2D::Core::IRenderBackend
{
	// Configurable results
	DF2D::Data::TextureID nextRenderTargetId{1};

	DF2D::Core::Color displayColor{};

	DF2D::Core::Vector2I resolutionTarget{1920, 1080};


	// Call tracking
	int createRenderTargetCount{0};

	int destroyTextureCount{0};

	int presentCount{0};

	int drawCount{0};

	DF2D::Core::Vector2I lastRenderTargetSize{};

	DF2D::Data::TextureID lastDestroyedTexture{0};

	DF2D::Core::RectI lastViewport{};


	void DrawPixel(const DF2D::Core::Vector2F&, DF2D::Core::Color) override
	{
		drawCount++;
	}

	void DrawLine(const DF2D::Core::Vector2F&, const DF2D::Core::Vector2F&, DF2D::Core::Color) override
	{
		drawCount++;
	}

	void DrawRect(DF2D::Core::RectF, float, DF2D::Core::Color, bool) override
	{
		drawCount++;
	}

	void DrawCircle(const DF2D::Core::Vector2F&, float, DF2D::Core::Color, bool) override
	{
		drawCount++;
	}

	void DrawTexture(
		DF2D::Data::TextureID,
		const std::optional<DF2D::Core::RectI>&,
		const std::optional<DF2D::Core::RectF>&,
		const std::optional<DF2D::Core::Vector2F>&,
		float,
		DF2D::Data::RenderFlip,
		DF2D::Core::Color) override
	{
		drawCount++;
	}

	void SetRenderTarget(DF2D::Data::TextureID) override
	{
	}

	void ClearCurrentRenderTarget() override
	{
	}

	void Present() override
	{
		presentCount++;
	}

	DF2D::Data::TextureID CreateRenderTarget(int w, int h) override
	{
		createRenderTargetCount++;
		lastRenderTargetSize = { w, h };

		return nextRenderTargetId;
	}

	void DestroyTexture(DF2D::Data::TextureID id) override
	{
		destroyTextureCount++;
		lastDestroyedTexture = id;
	}

	DF2D::Core::Color GetDisplayColor() override
	{
		return displayColor;
	}

	DF2D::Core::Vector2I GetResolutionTarget() override
	{
		return resolutionTarget;
	}

	void SetViewport(DF2D::Core::RectI viewPort) override
	{
		lastViewport = viewPort;
	}

	void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override
	{
		displayColor = { r, g, b, a };
	}

	void SetResolutionTarget(DF2D::Core::Vector2I targetResolution) override
	{
		resolutionTarget = targetResolution;
	}
};
