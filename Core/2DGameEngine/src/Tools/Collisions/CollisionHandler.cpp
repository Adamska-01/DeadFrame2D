#include "Components/Collisions/BoxCollider2D.h"
#include "Components/Collisions/CircleCollider2D.h"
#include "Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "Components/Transform.h"
#include "Data/Collision/CollisionInfo.h"
#include "Tools/Collisions/CollisionHandler.h"
#include <algorithm>


SDL_Rect CollisionHandler::GetExpandedTarget(const SDL_Rect* colliderRect, const SDL_Rect* tileRect)
{
	return SDL_Rect {
		tileRect->x - colliderRect->w / 2,
		tileRect->y - colliderRect->h / 2,
		tileRect->w + colliderRect->w,
		tileRect->h + colliderRect->h
	};
}

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
	contact_normal = { 0.0f, 0.0f };
	contact_point = { 0.0f, 0.0f };

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
		contact_normal = invdir.y < 0 ? Vector2F(0,  1) : Vector2F(0, -1);
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

	box->OnCollisionCallback(CollisionInfo(normal, other->GetGameObject()));

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
	const auto& previousRect = box->GetPreviousBox();
	const auto& colliderRect = box->GetCollisionBox();
	const auto& collisionLayers = other->GetCollisionLayers();
	const auto& tileMapDimension = other->GetTileMapDimensions();
	auto tileSize = other->GetTileSize();
	auto boxStartFramePosition = box->GetStartFramePosition();
	auto boxTransform = box->GetTranform();

	auto left_tile = std::max(0, std::min(previousRect.x, colliderRect.x) / tileSize);
	auto right_tile = std::min(tileMapDimension.x, std::max(previousRect.x + previousRect.w, colliderRect.x + colliderRect.w) / tileSize);
	auto top_tile = std::max(0, std::min(previousRect.y, colliderRect.y) / tileSize);
	auto bottom_tile = std::min(tileMapDimension.y, std::max(previousRect.y + previousRect.h, colliderRect.y + colliderRect.h) / tileSize);
	
	auto startOrigin = Vector2F(previousRect.x + previousRect.w / 2.0f, previousRect.y + previousRect.h / 2.0f);
	auto velocity = boxTransform->position - boxStartFramePosition;

	if (velocity.IsZero())
		return false;

	std::vector<std::pair<int, std::tuple<float, SDL_Rect>>> collisions;

	// Get Collisions
	for (auto i = left_tile; i <= right_tile; i++)
	{
		for (auto j = top_tile; j <= bottom_tile; j++)
		{
			auto withinBounds = i >= 0 && i < tileMapDimension.x && j >= 0 && j < tileMapDimension.y;
			
			if (!withinBounds)
				continue;
			
			for (const auto& layer : collisionLayers)
			{
				if (layer.Data[j][i] == 0)
					continue;
				
				SDL_Rect tileRect = { i * tileSize, j * tileSize, tileSize, tileSize };
				SDL_Rect expandedTarget = GetExpandedTarget(&colliderRect, &tileRect);

				Vector2F contactPoint, normal;
				float t = 0.0f;

				if (RayVsRect(startOrigin, velocity, &expandedTarget, contactPoint, normal, t) && t >= 0.0f && t < 1.0f)
				{
					collisions.emplace_back(i, std::make_tuple(t, tileRect));
				}
				
				break;
			}
		}
	}

	// Do the sort
	std::sort(collisions.begin(), collisions.end(), [](const auto& a, const auto& b)
		{
			return std::get<0>(a.second) < std::get<0>(b.second);
		});

	// Resolve Collisions
	for (const auto& [_, data] : collisions)
	{
		auto& [_, targetRect] = data;

		SDL_Rect expandedTarget = GetExpandedTarget(&colliderRect, &targetRect);
		Vector2F contactPoint, normal;
		float t = 0.0f;

		if (RayVsRect(startOrigin, velocity, &expandedTarget, contactPoint, normal, t) && t >= 0.0f && t < 1.0f)
		{
			velocity -= normal * velocity.Dot(normal) * (1.0f - t);
		}
	}

	boxTransform->position = boxStartFramePosition + velocity;
	
	//box->OnCollisionCallback(CollisionInfo(normal, other->OwningObject));
	
	return true;
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
	const auto& collisionLayers = other->GetCollisionLayers();
	const auto& tileMapDimension = other->GetTileMapDimensions();
	const auto tileSize = other->GetTileSize();
	const auto& startFrameCircle = circle->GetStartFrameCircle();
	const auto radius = circle->GetCircle().radius;
	auto circleEndFramePosition = circle->GetCircle().position;

	auto velocity = circleEndFramePosition - startFrameCircle.position;
	if (velocity.IsZero())
		return false;

	// Compute tile bounds in which to check collisions (inclusive)
	auto left_tile = std::max(0, static_cast<int>((std::min(startFrameCircle.position.x, circleEndFramePosition.x) - radius) / tileSize));
	auto right_tile = std::min(tileMapDimension.x - 1, static_cast<int>((std::max(startFrameCircle.position.x, circleEndFramePosition.x) + radius) / tileSize));
	auto top_tile = std::max(0, static_cast<int>((std::min(startFrameCircle.position.y, circleEndFramePosition.y) - radius) / tileSize));
	auto bottom_tile = std::min(tileMapDimension.y - 1, static_cast<int>((std::max(startFrameCircle.position.y, circleEndFramePosition.y) + radius) / tileSize));

	auto requiresAdjustments = false;

	for (auto j = top_tile; j <= bottom_tile; j++)
	{
		for (auto i = left_tile; i <= right_tile; i++)
		{
			auto isSolidTile = false;
			for (const auto& layer : collisionLayers)
			{
				isSolidTile = layer.Data[j][i] > 0;

				if (isSolidTile) 
					break;
			}

			if (!isSolidTile) 
				continue;

			// Find the nearest point on the tile's AABB to the circle center
			Vector2F nearestPoint
			{
				std::max(float(i * tileSize), std::min(circleEndFramePosition.x, float((i + 1) * tileSize))),
				std::max(float(j * tileSize), std::min(circleEndFramePosition.y, float((j + 1) * tileSize)))
			};

			// Vector from circle to nearest point
			auto delta = nearestPoint - circleEndFramePosition;
			auto distance = delta.Magnitude();
			auto overlap = radius - distance;

			// Check overlap
			if (overlap > 0 && distance > 0.0001f)
			{
				circleEndFramePosition -= (delta / distance) * overlap;

				requiresAdjustments = true;
			}
		}
	}

	if (!requiresAdjustments)
		return false;

	// Assuming the circle position is the same as the transform position (TODO: Add an offset buffer to the circle collider) 
	circle->SetPos(circleEndFramePosition);
	circle->GetTranform()->position = circleEndFramePosition;

	///box->OnCollisionCallback(CollisionInfo(normal, other->OwningObject));
	
	return true;
}


bool CollisionHandler::Visit(TiledMapCompatibleCollider2D* tile, BoxCollider2D* other)
{
	return Visit(other, tile);
}

bool CollisionHandler::Visit(TiledMapCompatibleCollider2D* tile, CircleCollider2D* other)
{
	return Visit(other, tile);
}