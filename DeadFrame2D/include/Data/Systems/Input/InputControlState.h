#pragma once
#include <ostream>


namespace DeadFrame2D::Data
{
	struct InputControlState
	{
		float value = 0.0f;

		bool pressed = false;

		bool held = false;

		bool released = false;


		inline friend std::ostream& operator<<(std::ostream& out, const InputControlState& s)
		{
			out << "InputControlState { "
				<< "value: " << s.value
				<< ", pressed: " << std::boolalpha << s.pressed
				<< ", held: " << s.held
				<< ", released: " << s.released
				<< " }";

			return out;
		}
	};
}