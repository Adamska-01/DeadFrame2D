#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Component/Collisions/Collider2D.h"


namespace DF2D::Engine
{
	template <typename TRenderer>
	class TileCollider2D : public Collider2D
	{
		TYPE_INFO(TileCollider2D, Collider2D);


	protected:
		ComponentHandle<TRenderer> tileMapRenderer;


		TileCollider2D(const Data::PhysicsMaterial& physicsMaterial = Data::PhysicsMaterial())
			: Collider2D(physicsMaterial)
		{
		}

		virtual ~TileCollider2D() = default;
	};
}