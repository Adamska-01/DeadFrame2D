#pragma once
#include "Engine/ECS/Entity/Object/Core/GameObject.h"


// Exposes the Scene-only construction seam (BindToScene/ConstructGameObject) so tests
// can produce a fully wired GameObject without a real Scene/SceneManager.
class TestGameObject : public DF2D::Engine::GameObject
{
public:
	using GameObject::BindToScene;
	using GameObject::ConstructGameObject;
};