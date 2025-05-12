#pragma once


/**
 * @brief Enum representing the type of a 2D physics body.
 */
enum class BodyType2D
{
	/**
	 * @brief Static bodies do not move under simulation and are not affected by forces or collisions.
	 *        Used for immovable objects like the ground or walls.
	 */
	Static = 0,

	/**
	 * @brief Kinematic bodies are moved manually by setting their velocity.
	 *        They are not affected by forces or collisions but can affect dynamic bodies.
	 */
	Kinematic,

	/**
	 * @brief Dynamic bodies are fully simulated, react to forces, and collide with other bodies.
	 *        Used for most moving objects in the world.
	 */
	Dynamic
};