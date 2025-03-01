#include "Components/Collisions/BoxCollider2D.h"
#include "Components/Collisions/CircleCollider2D.h"
#include "Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "Data/Collision/CollisionInfo.h"
#include "Tools/Collisions/CollisionHandler.h"



bool CollisionHandler::PointVsBox(const Vector2F& point, const SDL_Rect* box)
{
	return point.x >= box->x 
		&& point.y >= box->y 
		&& point.x < box->x + box->w 
		&& point.y < box->y + box->h;
}

bool CollisionHandler::PointVsCircle(const Vector2F& point, const Circle* circle)
{
	auto dx = point.x - circle->position.x;
	auto dy = point.y - circle->position.y;

	return (dx * dx + dy * dy) <= (circle->radius * circle->radius);
}

bool CollisionHandler::RectVsRect(const SDL_Rect* boxA, const SDL_Rect* boxB)
{
	return boxA->x < boxB->x + boxB->w 
		&& boxB->x < boxA->x + boxA->w
		&& boxA->y < boxB->y + boxB->h 
		&& boxB->y < boxA->y + boxA->h;
}

bool CollisionHandler::RayVsRect(const Vector2F& ray_origin, const Vector2F& ray_dir, const SDL_Rect* target, Vector2F& contact_point, Vector2F& contact_normal, float& t_hit_near)
{
	contact_normal = { 0,0 };
	contact_point = { 0,0 };

	// Cache division
	auto invdir = Vector2F(1.0f / ray_dir.x, 1.0f / ray_dir.y);

	// Calculate intersections with rectangle bounding axes
	auto t_near = (Vector2F(target->x, target->y) - ray_origin) * invdir;
	auto t_far = (Vector2F(target->x, target->y) + Vector2F(target->w, target->h) - ray_origin) * invdir;

	if (std::isnan(t_far.y) || std::isnan(t_far.x) || std::isnan(t_near.y) || std::isnan(t_near.x)) 
		return false;

	// Sort distances
	if (t_near.x > t_far.x)
	{
		std::swap(t_near.x, t_far.x);
	}
	
	if (t_near.y > t_far.y)
	{
		std::swap(t_near.y, t_far.y);
	}

	// Early rejection
	if (t_near.x > t_far.y || t_near.y > t_far.x) 
		return false;

	// Closest 'time' will be the first contact
	t_hit_near = std::max(t_near.x, t_near.y);

	// Furthest 'time' is contact on opposite side of target
	auto t_hit_far = std::min(t_far.x, t_far.y);

	// Reject if ray direction is pointing away from object
	if (t_hit_far < 0)
		return false;

	// Contact point of collision from parametric line equation
	contact_point = ray_origin + ray_dir * t_hit_near;

	if (t_near.x > t_near.y)
	{
		contact_normal = invdir.x < 0 ? Vector2F(1, 0) : Vector2F(-1, 0);
	}
	else if (t_near.x < t_near.y)
	{
		contact_normal = invdir.y < 0 ? Vector2F(0, 1) : Vector2F(0, -1);
	}

	// Note if t_near == t_far, collision is principly in a diagonal
	// so pointless to resolve. By returning a CN={0,0} even though its
	// considered a hit, the resolver wont change anything.
	return true;
}

bool CollisionHandler::Visit(BoxCollider2D* box, BoxCollider2D* other)
{
	auto A = box->GetCollisionBox();
	auto B = other->GetCollisionBox();

	auto leftA = A.x, rightA = A.x + A.w, topA = A.y, bottomA = A.y + A.h;
	auto leftB = B.x, rightB = B.x + B.w, topB = B.y, bottomB = B.y + B.h;

	// Check for no collision
	if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
		return false;

	auto overlapX = std::min(rightA, rightB) - std::max(leftA, leftB);
	auto overlapY = std::min(bottomA, bottomB) - std::max(topA, topB);

	auto normal = Vector2F
	{ 
		(overlapX < overlapY) ? (A.x < B.x ? -1.0f : 1.0f) : 0.0f,
		(overlapX >= overlapY) ? (A.y < B.y ? -1.0f : 1.0f) : 0.0f
	};

	box->OnCollisionCallback(CollisionInfo(normal, other->OwningObject));

	return true;
}

bool CollisionHandler::Visit(BoxCollider2D* box, CircleCollider2D* other)
{
	auto A = other->GetCircle();
	const auto& B = box->GetCollisionBox();

	// Closest points on collision box
	Vector2F closestPoint;

	// Find closest x offset
	if (A.position.x < B.x)
		closestPoint.x = B.x;
	else if (A.position.x > B.x + B.w)
		closestPoint.x = B.x + B.w;
	else
		closestPoint.x = A.position.x;

	// Find closest y offset
	if (A.position.y < B.y)
		closestPoint.y = B.y;
	else if (A.position.y > B.y + B.h)
		closestPoint.y = B.y + B.h;
	else
		closestPoint.y = A.position.y;

	//collisionPoint = closestPoint;

	// Check if the point is inside the circle
	if (A.position.Distance(closestPoint) < A.radius)
		return true;

	// No collision
	return false;
}

bool CollisionHandler::Visit(BoxCollider2D* box, TiledMapCompatibleCollider2D* other)
{
	const auto& colliderRect = box->GetCollisionBox();
	const auto& collisionLayers = other->GetCollisionLayers();
	const auto& tilesize = other->GetTileSize();
	const auto& tileMapDimension = other->GetTileMapDimensions();

	auto left_tile = colliderRect.x / tilesize;
	auto right_tile = (colliderRect.x + colliderRect.w) / tilesize;

	auto top_tile = colliderRect.y / tilesize;
	auto bottom_tile = (colliderRect.y + colliderRect.h) / tilesize;

	if (top_tile < 0)
		top_tile = 0;

	if (right_tile > tileMapDimension.x)
		right_tile = tileMapDimension.x;

	if (top_tile < 0)
		top_tile = 0;

	if (bottom_tile > tileMapDimension.y)
		bottom_tile = tileMapDimension.y;

	for (auto i = left_tile; i <= right_tile; i++)
	{
		for (auto j = top_tile; j <= bottom_tile; j++)
		{
			auto withinBounds = i >= 0 && i < tileMapDimension.x && j >= 0 && j < tileMapDimension.y;
			
			if (!withinBounds)
				continue;

			auto isSolidTile = false;
		
			for (const auto& layer : collisionLayers)
			{
				isSolidTile = layer.Data[j][i] > 0;

				if (isSolidTile)
					break;
			}

			// This is wrong
			if (isSolidTile)
			{
				auto tileCenterX = (i + 0.5f) * tilesize;
				auto tileCenterY = (j + 0.5f) * tilesize;

				auto colliderCenterX = colliderRect.x + colliderRect.w / 2.0f;
				auto colliderCenterY = colliderRect.y + colliderRect.h / 2.0f;

				auto diffX = colliderCenterX - tileCenterX;
				auto diffY = colliderCenterY - tileCenterY;

				auto overlapX = (tilesize / 2.0f + colliderRect.w / 2.0f) - std::abs(diffX);
				auto overlapY = (tilesize / 2.0f + colliderRect.h / 2.0f) - std::abs(diffY);

				auto normal = Vector2F{
					(overlapX < overlapY) ? (diffX < 0 ? -1.0f : 1.0f) : 0.0f,
					(overlapX >= overlapY) ? (diffY < 0 ? -1.0f : 1.0f) : 0.0f
				};

				box->OnCollisionCallback(CollisionInfo(normal, other->OwningObject));

				return true;
			}
		}
	}

	return false;
}

bool CollisionHandler::Visit(CircleCollider2D* circle, BoxCollider2D* other)
{
	return Visit(other, circle);
}

bool CollisionHandler::Visit(CircleCollider2D* circle, CircleCollider2D* other)
{
	auto A = circle->GetCircle();
	auto B = other->GetCircle();

	// Check if the point is touching the circle
	if (A.position.Distance(B.position) <= A.radius + B.radius)
		return true;

	// No collision
	return false;
}

bool CollisionHandler::Visit(CircleCollider2D* circle, TiledMapCompatibleCollider2D* other)
{
	auto collisionLayers = other->GetCollisionLayers();
	auto colliderCircle = circle->GetCircle();
	auto circleCenter = colliderCircle.position;
	auto circleRadius = colliderCircle.radius;

	// Replace these with the real values
	auto tilesize = 32;
	auto rowCount = 20;
	auto colCount = 60;

	// We need to iterate over the tiles within the circle's radius
	// Find the bounding box of the circle in terms of tiles (like how we did for the BoxCollider2D)
	int left_tile = (circleCenter.x - circleRadius) / tilesize;
	int right_tile = (circleCenter.x + circleRadius) / tilesize;
	int top_tile = (circleCenter.y - circleRadius) / tilesize;
	int bottom_tile = (circleCenter.y + circleRadius) / tilesize;

	// Clamping to bounds
	if (left_tile < 0) left_tile = 0;
	if (right_tile > colCount) right_tile = colCount;
	if (top_tile < 0) top_tile = 0;
	if (bottom_tile > rowCount) bottom_tile = rowCount;

	// Check all tiles within the circle's bounding box
	for (auto i = left_tile; i <= right_tile; i++)
	{
		for (auto j = top_tile; j <= bottom_tile; j++)
		{
			// Check if the tile is within the circle's area
			auto withinBounds = i >= 0 && i < colCount && j >= 0 && j < rowCount;

			if (withinBounds)
			{
				// Get the tile center position
				Vector2F tileCenter(i * tilesize + tilesize / 2, j * tilesize + tilesize / 2);

				// Check if the tile's center is within the circle's radius
				auto dx = tileCenter.x - circleCenter.x;
				auto dy = tileCenter.y - circleCenter.y;
				auto distanceSquared = dx * dx + dy * dy;

				std::cout << "Distance Squared: " << distanceSquared << std::endl;

				if (distanceSquared <= circleRadius * circleRadius)
				{
					// If the tile is within the circle's radius, check if it's a solid tile
					auto isSolidTile = false;
					for (const auto& layer : collisionLayers)
					{
						isSolidTile = layer.Data[j][i] > 0;
						
						if (isSolidTile) 
							break;
					}

					if (isSolidTile)
						return true;
				}
			}
		}
	}

	return false;
}

bool CollisionHandler::Visit(TiledMapCompatibleCollider2D* tile, BoxCollider2D* other)
{
	return Visit(other, tile);
}

bool CollisionHandler::Visit(TiledMapCompatibleCollider2D* tile, CircleCollider2D* other)
{
	return Visit(other, tile);
}