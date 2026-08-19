#include <doctest.h>
#include "Models/Input/InputBinding.h"
#include <nlohmann/json.hpp>


using namespace DF2D::Models;


TEST_SUITE_BEGIN("InputBinding");


TEST_CASE("Simple keyboard binding round-trips through JSON")
{
	InputBinding binding;
	binding.inputDeviceType = InputDeviceType::KEYBOARD;
	binding.bindingType = BindingType::SIMPLE;
	binding.controlType = InputControlType::DIGITAL;
	binding.value = static_cast<int>(KeyboardKeyCode::SPACE);

	nlohmann::json j = binding;
	InputBinding parsed = j.get<InputBinding>();

	REQUIRE(std::holds_alternative<int>(parsed.value));
	CHECK(static_cast<KeyboardKeyCode>(std::get<int>(parsed.value)) == KeyboardKeyCode::SPACE);
}


TEST_CASE("Simple mouse analog binding round-trips through JSON")
{
	InputBinding binding;
	binding.inputDeviceType = InputDeviceType::MOUSE;
	binding.bindingType = BindingType::SIMPLE;
	binding.controlType = InputControlType::ANALOG;
	binding.value = static_cast<int>(MouseAxisCode::MOTION_X);

	nlohmann::json j = binding;
	InputBinding parsed = j.get<InputBinding>();

	REQUIRE(std::holds_alternative<int>(parsed.value));
	CHECK(static_cast<MouseAxisCode>(std::get<int>(parsed.value)) == MouseAxisCode::MOTION_X);
}


TEST_CASE("Simple controller digital binding round-trips through JSON")
{
	InputBinding binding;
	binding.inputDeviceType = InputDeviceType::CONTROLLER;
	binding.bindingType = BindingType::SIMPLE;
	binding.controlType = InputControlType::DIGITAL;
	binding.value = static_cast<int>(ControllerButtonCode::A);

	nlohmann::json j = binding;
	InputBinding parsed = j.get<InputBinding>();

	REQUIRE(std::holds_alternative<int>(parsed.value));
	CHECK(static_cast<ControllerButtonCode>(std::get<int>(parsed.value)) == ControllerButtonCode::A);
}


TEST_CASE("Composite1D keyboard binding round-trips through JSON")
{
	InputBinding binding;
	binding.inputDeviceType = InputDeviceType::KEYBOARD;
	binding.bindingType = BindingType::COMPOSITE_1D;
	binding.controlType = InputControlType::DIGITAL;
	binding.value = Composite1D{ static_cast<int>(KeyboardKeyCode::A), static_cast<int>(KeyboardKeyCode::D) };

	nlohmann::json j = binding;
	InputBinding parsed = j.get<InputBinding>();

	REQUIRE(std::holds_alternative<Composite1D>(parsed.value));

	auto composite = std::get<Composite1D>(parsed.value);

	CHECK(static_cast<KeyboardKeyCode>(composite.negative) == KeyboardKeyCode::A);
	CHECK(static_cast<KeyboardKeyCode>(composite.positive) == KeyboardKeyCode::D);
}


TEST_CASE("Composite2D keyboard binding round-trips through JSON")
{
	InputBinding binding;
	binding.inputDeviceType = InputDeviceType::KEYBOARD;
	binding.bindingType = BindingType::COMPOSITE_2D;
	binding.controlType = InputControlType::DIGITAL;
	binding.value = Composite2D
	{
		static_cast<int>(KeyboardKeyCode::W),
		static_cast<int>(KeyboardKeyCode::S),
		static_cast<int>(KeyboardKeyCode::A),
		static_cast<int>(KeyboardKeyCode::D)
	};

	nlohmann::json j = binding;
	InputBinding parsed = j.get<InputBinding>();

	REQUIRE(std::holds_alternative<Composite2D>(parsed.value));

	auto composite = std::get<Composite2D>(parsed.value);

	CHECK(static_cast<KeyboardKeyCode>(composite.up) == KeyboardKeyCode::W);
	CHECK(static_cast<KeyboardKeyCode>(composite.down) == KeyboardKeyCode::S);
	CHECK(static_cast<KeyboardKeyCode>(composite.left) == KeyboardKeyCode::A);
	CHECK(static_cast<KeyboardKeyCode>(composite.right) == KeyboardKeyCode::D);
}


TEST_CASE("from_json throws when 'value' is missing")
{
	nlohmann::json j =
	{
		{"inputDeviceType", "Keyboard"},
		{"bindingType", "Simple"},
		{"controlType", "Digital"}
	};

	CHECK_THROWS_AS(j.get<InputBinding>(), std::runtime_error);
}


TEST_SUITE_END();