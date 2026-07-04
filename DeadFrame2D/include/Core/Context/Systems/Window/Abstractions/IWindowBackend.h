#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/Window/WindowMode.h"
#include "DF2D_API.h"
#include <string>


namespace DF2D::Core
{
	class DF2D_API IWindowBackend
	{
	public:
		virtual ~IWindowBackend() = default;


		virtual Core::Vector2I GetResolution() = 0;

		virtual void SetWindowTitle(const std::string& title) = 0;

		virtual void SetWindowIcon(std::string_view iconSource) = 0;

		virtual void SetWindowMode(Data::WindowMode mode) = 0;

		virtual bool SetResolution(Core::Vector2I resolution) = 0;
	};
}