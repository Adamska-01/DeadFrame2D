#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Layout/UIAnchor.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Button.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include <string>


namespace DF2D::Engine
{
	/**
	* @brief A button: a RectTransform anchored to a preset plus a Button, spawned in one call.
	*/
	class DF2D_API ButtonBlueprint : public GameObject
	{
	private:
		ComponentHandle<Button> button;


	public:
		ButtonBlueprint(
			const std::string& text,
			Data::UIAnchor anchor,
			const Core::Vector2F& anchoredPosition,
			const Core::Vector2F& size = { 200.0f, 56.0f });


		ComponentHandle<Button> GetButton() const;
	};
}