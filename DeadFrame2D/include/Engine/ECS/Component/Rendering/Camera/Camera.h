#pragma once
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
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
		static std::vector<Camera*> cameras;


		ComponentHandle<Transform> transform;

		Core::Renderer* renderer = nullptr;

		Core::Vector2I resolutionTarget;

		Core::RectF normalizedViewport;

		Data::TextureID renderTarget;

		float zoom;


		void RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	public:
		Camera();

		virtual ~Camera() override;


		virtual void Init() override;


		void SetZoom(float zoom);

		void SetViewport(const Core::RectF& normalizedViewport);

		float GetZoom() const;

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


		static const std::vector<Camera*>& GetCameras();
	};
}