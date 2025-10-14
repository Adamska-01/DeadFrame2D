#pragma once
#include "DF2D_API.h"
#include "Engine/Components/Collisions/Collider2D.h"


namespace DeadFrame2D::Engine
{
	template <typename TRenderer>
	class TileCollider2D : public Collider2D
	{
		TYPE_INFO(TileCollider2D, Collider2D);


	protected:
		ComponentHandle<TRenderer> tileMapRenderer;


		TileCollider2D(const DeadFrame2D::Data::PhysicsMaterial& physicsMaterial = DeadFrame2D::Data::PhysicsMaterial())
			: Collider2D(physicsMaterial)
		{
		}

		virtual ~TileCollider2D() = default;
	};
}