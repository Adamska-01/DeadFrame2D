#include "Converters/Physics/PhysicsConversions.h"
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsContactSink.h"
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsDebugDraw.h"
#include "Factories/Products/Context/Systems/Physics/Box2DPhysicsBackend.h"
#include <box2d/box2d.h>
#include <variant>


// -------------------------------------------------------------
// -------------------------- Helpers --------------------------
// -------------------------------------------------------------
namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Models;


	class Box2DContactAdapter : public b2ContactListener
	{
	private:
		IPhysicsContactSink* sink = nullptr;

		float pixelPerMeter = 1.0f;


		static std::pair<FixtureID, FixtureID> GetFixtureIDsFromContact(b2Contact* contact)
		{
			auto fixtureA = static_cast<FixtureID>(contact->GetFixtureA()->GetUserData().pointer);
			auto fixtureB = static_cast<FixtureID>(contact->GetFixtureB()->GetUserData().pointer);

			return { fixtureA, fixtureB };
		}


	public:
		Box2DContactAdapter(float pixelPerMeter)
			: pixelPerMeter(pixelPerMeter)
		{
		}


		void SetSink(IPhysicsContactSink* newSink)
		{
			sink = newSink;
		}


		void BeginContact(b2Contact* contact) override
		{
			if (sink == nullptr)
				return;

			auto [fixtureA, fixtureB] = GetFixtureIDsFromContact(contact);

			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);

			auto contactPoint = Vector2F(worldManifold.points[0].x * pixelPerMeter, worldManifold.points[0].y * pixelPerMeter);
			auto normal = Vector2F(worldManifold.normal.x, worldManifold.normal.y);

			sink->OnContactBegin(fixtureA, fixtureB, contactPoint, normal);
		}

		void EndContact(b2Contact* contact) override
		{
			if (sink == nullptr)
				return;

			auto [fixtureA, fixtureB] = GetFixtureIDsFromContact(contact);

			sink->OnContactEnd(fixtureA, fixtureB);
		}
	};


	class Box2DDebugDrawAdapter : public b2Draw
	{
	private:
		IPhysicsDebugDraw* target = nullptr;

		float pixelPerMeter = 1.0f;


		Vector2F ToPixels(const b2Vec2& point) const
		{
			return Vector2F(point.x * pixelPerMeter, point.y * pixelPerMeter);
		}

		static Color ToColor(const b2Color& color)
		{
			return Color
			{
				.r = uint8_t(color.r * 255),
				.g = uint8_t(color.g * 255),
				.b = uint8_t(color.b * 255),
				.a = uint8_t(color.a * 255)
			};
		}


	public:
		Box2DDebugDrawAdapter(float pixelPerMeter)
			: pixelPerMeter(pixelPerMeter)
		{
			SetFlags(
				b2Draw::e_shapeBit |
				b2Draw::e_jointBit |
				b2Draw::e_aabbBit |
				b2Draw::e_pairBit |
				b2Draw::e_centerOfMassBit);
		}


		void SetTarget(IPhysicsDebugDraw* newTarget)
		{
			target = newTarget;
		}


		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
		{
			if (target == nullptr)
				return;

			auto lineColor = ToColor(color);

			for (int32 i = 0; i < vertexCount; ++i)
			{
				auto p1 = vertices[i];
				auto p2 = vertices[(i + 1) % vertexCount];

				target->DrawSegment(ToPixels(p1), ToPixels(p2), lineColor);
			}
		}

		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
		{
			DrawPolygon(vertices, vertexCount, color);
		}

		void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override
		{
			if (target == nullptr)
				return;

			target->DrawCircle(ToPixels(center), radius * pixelPerMeter, /*filled*/ false, ToColor(color));
		}

		void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override
		{
			if (target == nullptr)
				return;

			target->DrawCircle(ToPixels(center), radius * pixelPerMeter, /*filled*/ false, ToColor(color));
		}

		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
		{
			if (target == nullptr)
				return;

			target->DrawSegment(ToPixels(p1), ToPixels(p2), ToColor(color));
		}

		void DrawTransform(const b2Transform& xf) override
		{
			DrawPoint(xf.p, 2.0f, b2Color{ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override
		{
			if (target == nullptr)
				return;

			target->DrawPoint(ToPixels(p), size, ToColor(color));
		}
	};
}


namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Models;


	Box2DPhysicsBackend::Box2DPhysicsBackend(const PhysicsConfig& physicsConfig)
		: nextBodyID(1),
		nextFixtureID(1),
		pixelPerMeter(physicsConfig.pixelPerMeter),
		meterPerPixel(physicsConfig.meterPerPixel)
	{
		world = std::make_unique<b2World>(b2Vec2(physicsConfig.gravityX, physicsConfig.gravityY));

		contactAdapter = std::make_unique<Box2DContactAdapter>(pixelPerMeter);

		debugDrawAdapter = std::make_unique<Box2DDebugDrawAdapter>(pixelPerMeter);

		world->SetContactListener(contactAdapter.get());

		world->SetDebugDraw(debugDrawAdapter.get());
	}

	Box2DPhysicsBackend::~Box2DPhysicsBackend()
	{
		contactAdapter->SetSink(nullptr);

		world.reset();
	}

	b2Body* Box2DPhysicsBackend::FindBody(BodyID body) const
	{
		auto found = bodies.find(body);

		return found != bodies.end() ? found->second : nullptr;
	}

	void Box2DPhysicsBackend::Step(float deltaTime, int velocityIterations, int positionIterations)
	{
		world->Step(deltaTime, velocityIterations, positionIterations);
	}

	void Box2DPhysicsBackend::SetGravity(const Vector2F& gravity)
	{
		world->SetGravity(b2Vec2(gravity.x, gravity.y));
	}

	BodyID Box2DPhysicsBackend::CreateBody(const BodyDefinition2D& bodyDefinition)
	{
		auto bodyDef = Physics::ToB2BodyDef(bodyDefinition, meterPerPixel);

		auto body = world->CreateBody(&bodyDef);

		if (body == nullptr)
			return 0;

		auto bodyID = nextBodyID++;

		bodies[bodyID] = body;

		return bodyID;
	}

	void Box2DPhysicsBackend::DestroyBody(BodyID body)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return;

		for (auto fixture = b2body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
			fixtures.erase(static_cast<FixtureID>(fixture->GetUserData().pointer));

		world->DestroyBody(b2body);

		bodies.erase(body);
	}

	FixtureID Box2DPhysicsBackend::CreateFixture(BodyID body, const PhysicsMaterial& physicsMaterial)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return 0;

		auto fixtureDef = Physics::ToB2FixtureDef(physicsMaterial);

		auto fixtureID = nextFixtureID++;

		fixtureDef.userData.pointer = static_cast<uintptr_t>(fixtureID);

		auto fixture = std::visit([&](const auto& shapeDefinition) -> b2Fixture*
			{
				using TShape = std::decay_t<decltype(shapeDefinition)>;

				if constexpr (std::is_same_v<TShape, BoxShapeDefinition2D>)
				{
					auto boxShape = Physics::ToB2BoxShape(shapeDefinition, meterPerPixel);

					fixtureDef.shape = &boxShape;

					return b2body->CreateFixture(&fixtureDef);
				}
				else
				{
					auto circleShape = Physics::ToB2CircleShape(shapeDefinition, meterPerPixel);

					fixtureDef.shape = &circleShape;

					return b2body->CreateFixture(&fixtureDef);
				}
			},
			physicsMaterial.shape);

		if (fixture == nullptr)
			return 0;

		fixtures[fixtureID] = fixture;

		return fixtureID;
	}

	void Box2DPhysicsBackend::DestroyFixture(FixtureID fixture)
	{
		auto found = fixtures.find(fixture);

		if (found == fixtures.end())
			return;

		auto b2fixture = found->second;

		fixtures.erase(found);

		b2fixture->GetBody()->DestroyFixture(b2fixture);
	}

	void Box2DPhysicsBackend::SetBodyEnabled(BodyID body, bool isEnabled)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return;

		b2body->SetEnabled(isEnabled);
	}

	void Box2DPhysicsBackend::SetBodyType(BodyID body, BodyType2D newBodyType)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return;

		b2body->SetType(Physics::ToB2BodyType(newBodyType));
	}

	void Box2DPhysicsBackend::SetBodyTransform(BodyID body, const Vector2F& position, float angle)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return;

		b2body->SetTransform(b2Vec2(position.x * meterPerPixel, position.y * meterPerPixel), angle);
	}

	BodyTransform2D Box2DPhysicsBackend::GetBodyTransform(BodyID body)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return BodyTransform2D{};

		auto position = b2body->GetPosition();

		return BodyTransform2D
		{
			.position = Vector2F(position.x * pixelPerMeter, position.y * pixelPerMeter),
			.angle = b2body->GetAngle()
		};
	}

	void Box2DPhysicsBackend::SetBodyAwake(BodyID body, bool isAwake)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return;

		b2body->SetAwake(isAwake);
	}

	void Box2DPhysicsBackend::SetBodyGravityScale(BodyID body, float gravityScale)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return;

		b2body->SetGravityScale(gravityScale);
	}

	Vector2F Box2DPhysicsBackend::GetLinearVelocity(BodyID body)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return Vector2F::Zero;

		auto velocity = b2body->GetLinearVelocity();

		return Vector2F(velocity.x, velocity.y);
	}

	void Box2DPhysicsBackend::SetLinearVelocity(BodyID body, const Vector2F& velocity)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return;

		b2body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	}

	void Box2DPhysicsBackend::ApplyLinearImpulseToCenter(BodyID body, const Vector2F& impulse)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return;

		b2body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
	}

	void Box2DPhysicsBackend::ApplyForceToCenter(BodyID body, const Vector2F& force)
	{
		auto b2body = FindBody(body);

		if (b2body == nullptr)
			return;

		b2body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
	}

	void Box2DPhysicsBackend::SetContactSink(IPhysicsContactSink* sink)
	{
		contactAdapter->SetSink(sink);
	}

	void Box2DPhysicsBackend::DebugDraw(IPhysicsDebugDraw& drawer)
	{
		debugDrawAdapter->SetTarget(&drawer);

		world->DebugDraw();

		debugDrawAdapter->SetTarget(nullptr);
	}
}
