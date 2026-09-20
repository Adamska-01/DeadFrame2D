#pragma once
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Components/Rendering/Camera/CameraFitMode.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"
#include <memory>
#include <vector>


namespace DF2D::Core
{
	struct Circle;
	class DeadFrameRuntime;
	class Renderer;
}


namespace DF2D::Engine
{
	class Transform;


	class DF2D_API Camera : public GameComponent
	{
		TYPE_INFO(Camera, GameComponent);


	private:
		static std::vector<ComponentHandle<Camera>> cameras;


		ComponentHandle<Transform> transform;

		Core::Renderer* renderer;

		Core::Vector2I resolutionTarget;

		Core::RectF normalizedViewport;

		Data::TextureID renderTarget;

		float zoom;

		Data::CameraFitMode fitMode;

		Core::Vector2I referenceResolution;

		/** @brief Additional scale applied when the render target differs from the reference resolution. */
		float viewScale;


		void RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		/** @brief Recomputes viewScale from the current fit mode, reference resolution and target size. */
		void RecomputeViewScale();


	public:
		Camera();

		virtual ~Camera() override;


		virtual void Init() override;


		void SetZoom(float zoom);

		void SetViewport(const Core::RectF& normalizedViewport);

		/** @brief Sets how the view scales when the render target's resolution isn't referenceResolution. */
		void SetFitMode(Data::CameraFitMode fitMode);

		/** @brief Sets the resolution the world is authored to look correct at. Ignored by ENVELOPE. */
		void SetReferenceResolution(const Core::Vector2I& referenceResolution);

		float GetZoom() const;

		Data::CameraFitMode GetFitMode() const;

		const Core::Vector2I& GetReferenceResolution() const;

		/** @brief The multiplier fit mode currently folds into zoom. 1 under ENVELOPE. */
		float GetViewScale() const;

		/** @brief Sets the normalized viewport (0-1 range). */
		const Core::RectF& GetViewport() const;

		/** @brief Returns the screen-space viewbox for this camera. */
		Core::RectF GetViewBox() const;

		/** @brief Returns the screen-space normalized viewbox for this camera. */
		Core::RectF GetNormalizedViewBox() const;

		Data::TextureID GetRenderTarget() const;

		/** @brief Converts world position to screen position. */
		Core::Vector2F WorldToScreen(const Core::Vector2F& worldPos) const;

		/** @brief Converts screen position to world position. */
		Core::Vector2F ScreenToWorld(const Core::Vector2F& screenPos) const;


		static const std::vector<ComponentHandle<Camera>>& GetCameras();
	};
}