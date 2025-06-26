#pragma once
#include <box2d/box2d.h>
#include <utility>


class ContactEventProvider;


/**
 * @brief Custom contact listener for Box2D collision events.
 *
 * This class receives callbacks from Box2D when two fixtures begin or end contact.
 * It can be used to dispatch OnCollisionEnter and OnCollisionExit events to your game objects.
 */
class ContactListener : public b2ContactListener
{
private:
	std::pair<ContactEventProvider*, ContactEventProvider*> GetUserDataFromContact(b2Contact* contact);


public:
	/**
	 * @brief Called when two fixtures begin to touch.
	 *
	 * Use this to trigger OnCollisionEnter events on your Collider2D.
	 *
	 * @param contact Pointer to the contact information between two fixtures.
	 */
	void BeginContact(b2Contact* contact) override;

	/**
	 * @brief Called when two fixtures cease to touch.
	 *
	 * Use this to trigger OnCollisionExit events on your Collider2D.
	 *
	 * @param contact Pointer to the contact information between two fixtures.
	 */
	void EndContact(b2Contact* contact) override;
};